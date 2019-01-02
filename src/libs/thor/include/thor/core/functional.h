#ifndef THOR_FUNCTIONAL_H_
#define THOR_FUNCTIONAL_H_

#include "platform/platform.h"
#include "allocator.h"
#include "assertion.h"

#ifndef USE_USER_FUNCTOR_ALLOCATOR
#define FUNCTOR_DEFAULT_ALLOCATOR Thor::Allocators::DefaultAllocator
#endif // !USE_USER_FUNCTOR_ALLOCATOR
 
namespace Thor
{
	template <int SIZE_IN_BYTES>
	struct FunctorStorage
	{
		static_assert(SIZE_IN_BYTES >= 0, "local buffer storage cannot have a negative size!");
		byte storage[SIZE_IN_BYTES];

		template<typename Ret>
		Ret& GetStorageTypeRef() const
		{
			return *reinterpret_cast<Ret*>(const_cast<byte*>(&storage[0]));
		}
	};

	#define THOR_FUNCTION_DEFAULT_SBO_SIZE (2 * sizeof(void*))

	template <typename Functor, int SIZE_IN_BYTES>
	struct IsFunctorInplaceAllocatable
	{
		static constexpr bool value =
			sizeof(Functor) <= sizeof(FunctorStorage<SIZE_IN_BYTES>) &&
			std::alignment_of_v<FunctorStorage<SIZE_IN_BYTES>> % std::alignment_of_v<Functor> == 0;
	};

	template <typename>
	class Function;

	template <typename ReturnType, typename... Args>
	class Function<ReturnType(Args...)>
	{
	public:

		Function() noexcept = default;

		Function(std::nullptr_t functor) noexcept {}

		Function(const Function& other)
		{
			if (this != &other)
			{
				Copy(other);
			}
		}

		Function(Function&& other)
		{
			if (this != &other)
			{
				Move(std::move(other));
			}
		}

		template <typename Functor>
		Function(Functor functor)
		{
			CreateForwardFunctor(std::move(functor));
		}

		~Function() noexcept
		{
			Destroy();
		}

		Function& operator=(const Function& other)
		{
			if (this != &other)
			{
				Destroy();
				Copy(other);
			}
			return *this;
		}

		Function& operator=(Function&& other)
		{
			if (this != &other)
			{
				Destroy();
				Move(std::move(other));
			}
			return *this;
		}

		Function& operator=(std::nullptr_t other) noexcept
		{
			Destroy();
			m_invokePtr = nullptr;
			m_managerFuncPtr = nullptr;
			return *this;
		}

		template <typename Functor>
		Function& operator=(Functor&& functor)
		{
			CreateForwardFunctor(std::move(functor));
			return *this;
		}

		explicit operator bool() const noexcept
		{
			return HasManager();
		}

		ReturnType operator()(Args... args) const
		{
			return (*m_invokePtr)(m_storage, std::forward<Args>(args)...);
		}

	private:

		using FunctorStorageType = FunctorStorage<THOR_FUNCTION_DEFAULT_SBO_SIZE>;
		FunctorStorageType m_storage;

		enum class FunctorManagerOperations
		{
			K_DESTROY,
			K_COPY,
			K_MOVE
		};

		// Functor allocated inplace.
		template<typename Functor, typename = void>
		struct FunctorManager
		{
			static Functor* GetFunctorPtr(const FunctorStorageType& storage) noexcept
			{
				return &(storage.GetStorageTypeRef<Functor>());
			}

			template<typename T>
			static void CreateFunctor(FunctorStorageType& storage, T&& functor)
			{
				::new (GetFunctorPtr(storage)) Functor(std::forward<T>(functor));
			}

			static void DestroyFunctor(FunctorStorageType& storage)
			{
				GetFunctorPtr(storage)->~Functor();
			}

			static void CopyFunctor(FunctorStorageType& to, const FunctorStorageType& from)
			{
				::new (GetFunctorPtr(to)) Functor(*GetFunctorPtr(from));
			}

			static void MoveFunctor(FunctorStorageType& to, FunctorStorageType& from)
			{
				::new (GetFunctorPtr(to)) Functor(std::move(*GetFunctorPtr(from)));
			}

			static void Manager(typename FunctorManagerOperations operation, void* to, void* from) noexcept
			{
				switch (operation)
				{
				case FunctorManagerOperations::K_DESTROY:
					DestroyFunctor(*static_cast<FunctorStorageType*>(to));
					break;
				case FunctorManagerOperations::K_COPY:
					CopyFunctor(*static_cast<FunctorStorageType*>(to), *static_cast<const FunctorStorageType*>(from));
					break;
				case FunctorManagerOperations::K_MOVE:
					CopyFunctor(*static_cast<FunctorStorageType*>(to), *static_cast<FunctorStorageType*>(from));
					DestroyFunctor(*static_cast<FunctorStorageType*>(from));
					break;
				default:
					break;
				}
			}

			static ReturnType Invoker(const FunctorStorageType& storage, Args... args)
			{
				return (*GetFunctorPtr(storage))(std::forward<Args>(args)...);
			}
		};
		
		//Functor allocated on heap
		template<typename Functor>
		struct FunctorManager<Functor, typename std::enable_if<!IsFunctorInplaceAllocatable<Functor, THOR_FUNCTION_DEFAULT_SBO_SIZE>::value>::type>
		{
			static Functor* GetFunctorPtr(const FunctorStorageType& storage) noexcept
			{
				return storage.template GetStorageTypeRef<Functor*>();
			}

			static Functor*& GetFunctorPtrRef(const FunctorStorageType& storage) noexcept
			{
				return storage.template GetStorageTypeRef<Functor*>();
			}

			template<typename T>
			static void CreateFunctor(FunctorStorageType& storage, T&& functor)
			{
				FUNCTOR_DEFAULT_ALLOCATOR allocator;
				Functor* func = static_cast<Functor*>(allocator.Allocate(sizeof(Functor), alignof(Functor)));				
				//T_ASSERT(func != nullptr, "Allocation failed!");
				T_ASSERT(func != nullptr);
				::new (static_cast<void*>(func)) Functor(std::forward<T>(functor));
				GetFunctorPtrRef(storage) = func;
			}

			static void DestroyFunctor(FunctorStorageType& storage)
			{
				Functor* func = GetFunctorPtr(storage);
				if (func)
				{
					FUNCTOR_DEFAULT_ALLOCATOR allocator;
					func->~Functor();
					allocator.Free(static_cast<void*>(func));
				}
			}

			static void CopyFunctor(FunctorStorageType& to, const FunctorStorageType& from)
			{
				FUNCTOR_DEFAULT_ALLOCATOR allocator;
				Functor* func = static_cast<Functor*>(allocator.Allocate(sizeof(Functor), alignof(Functor)));
				//T_ASSERT(func != nullptr, "Allocation failed!");
				T_ASSERT(func != nullptr);
				::new (static_cast<void*>(func)) Functor(*GetFunctorPtr(from));
				GetFunctorPtrRef(to) = func;
			}

			static void MoveFunctor(FunctorStorageType& to, FunctorStorageType& from)
			{
				Functor* func = GetFunctorPtr(from);
				GetFunctorPtrRef(to) = func;
				GetFunctorPtrRef(from) = nullptr;
			}

			static void Manager(typename FunctorManagerOperations operation, void* to, void* from) noexcept
			{
				switch (operation)
				{
					case FunctorManagerOperations::K_DESTROY:
					{
						DestroyFunctor(*static_cast<FunctorStorageType*>(to));
					}
					break;
					case FunctorManagerOperations::K_COPY:
					{
						CopyFunctor(*static_cast<FunctorStorageType*>(to), *static_cast<const FunctorStorageType*>(from));
					}
					break;
					case FunctorManagerOperations::K_MOVE:
					{
						CopyFunctor(*static_cast<FunctorStorageType*>(to), *static_cast<FunctorStorageType*>(from));
						// Moved ptr, no need to destruct ourselves
					}
					break;
					default:
					break;
				}
			}

			static ReturnType Invoker(const FunctorStorageType& storage, Args... args)
			{
				return (*GetFunctorPtr(storage))(std::forward<Args>(args)...);
			}
		};

	private:

		template <typename Functor>
		void CreateForwardFunctor(Functor&& functor)
		{
			using DecayedFunctorType = typename std::decay<Functor>::type;
			using FunctorManagerType = typename FunctorManager<DecayedFunctorType>;

			m_invokePtr = &FunctorManagerType::Invoker;
			m_managerFuncPtr = &FunctorManagerType::Manager;
			FunctorManagerType::CreateFunctor(m_storage, std::forward<Functor>(functor));
		}
		
		bool HasManager() const noexcept
		{
			return (m_managerFuncPtr != nullptr);
		}

		void Destroy() noexcept
		{
			if (HasManager())
			{
				(*m_managerFuncPtr)(FunctorManagerOperations::K_DESTROY, static_cast<void*>(&m_storage), nullptr);
			}
		}

		void Copy(const Function& other)
		{
			if (other.HasManager())
			{
				(*other.m_managerFuncPtr)(FunctorManagerOperations::K_COPY, static_cast<void*>(&m_storage), 
					const_cast<void*>(static_cast<const void*>(&other.m_storage)));
			}

			m_managerFuncPtr = other.m_managerFuncPtr;
			m_invokePtr = other.m_invokePtr;
		}

		void Move(Function&& other)
		{
			if (other.HasManager())
			{
				(*other.m_managerFuncPtr)(FunctorManagerOperations::K_MOVE, static_cast<void*>(&m_storage), static_cast<void*>(&other.m_storage));
			}
			m_managerFuncPtr = other.m_managerFuncPtr;
			m_invokePtr = other.m_invokePtr;
			other.m_managerFuncPtr = nullptr;
			other.m_invokePtr = nullptr;
		}
		
	private:

		typedef ReturnType(*InvokeFuncPtr)(const FunctorStorageType&, Args...);
		typedef void(*ManagerFuncPtr)(typename FunctorManagerOperations, void*, void*);

		InvokeFuncPtr m_invokePtr = nullptr;
		ManagerFuncPtr m_managerFuncPtr = nullptr;

	};
}
 
#endif // THOR_FUNCTIONAL_H_
