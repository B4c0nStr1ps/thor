#ifndef THOR_SMART_POINTERS_H_
#define THOR_SMART_POINTERS_H_

#include "allocator.h"
#include "memory.h"
#include "assertion.h"

#include <type_traits>

namespace Thor
{
	namespace TypeTraits
	{
		/// SharedRefTraits
		/// This exists for the sole purpose of creating a typedef called
		/// ReferenceType which is specialized for type void. The reason
		/// for this is that shared_ptr::operator*() returns a reference
		/// to T but if T is void, it needs to return void, not *void, 
		/// as the latter is not valid C++.
		template <typename T> struct SharedRefTraits
		{
			typedef T& ReferenceType;
		};

		template <> struct SharedRefTraits<void>
		{
			typedef void ReferenceType;
		};

		template <> struct SharedRefTraits<void const>
		{
			typedef void ReferenceType;
		};

		template <> struct SharedRefTraits<void volatile>
		{
			typedef void ReferenceType;
		};

		template <> struct SharedRefTraits<void const volatile>
		{
			typedef void ReferenceType;
		};
	}

	template<size_t Size, size_t Alignment>
	struct alignas(Alignment) AlignedStorageHelper { struct Type { byte m_data[Size]; }; };

	template<size_t Size, size_t Alignment>
	struct AlignedStorage
	{
		typedef typename AlignedStorageHelper<Size, Alignment>::Type Type;
	};

	struct RefCount
	{
	public:

		RefCount(int32 refCount = 1, int32 weakRefCount = 1) noexcept
			: m_refCount(refCount), m_weakRefCount(weakRefCount)
		{}

		FORCEINLINE void AddRef()
		{
			++m_refCount;
			++m_weakRefCount;
		}

		FORCEINLINE void Relese()
		{
			T_ASSERT(m_refCount > 0);
			T_ASSERT(m_weakRefCount > 0);
			--m_refCount;
			if (m_refCount > 0)
			{
				--m_weakRefCount;
			}
			else
			{
				FreeValue();
				--m_weakRefCount;
				if (m_weakRefCount == 0)
				{
					FreeRefCount();
				}
			}
		}

		FORCEINLINE void WeakAddRef()
		{
			++m_weakRefCount;
		}

		FORCEINLINE void WeakRelese()
		{
			T_ASSERT(m_weakRefCount > 0);
			--m_weakRefCount;
			if (m_weakRefCount == 0)
			{
				FreeRefCount();
			}
		}

		FORCEINLINE int32 GetCount() const
		{
			return m_refCount;
		}

		virtual void FreeValue() = 0;

		virtual void FreeRefCount() = 0;

	protected:

		int32 m_refCount;
		int32 m_weakRefCount;
	};
	
	template<typename T, typename Allocator>
	class RefCountPtr : public RefCount
	{
	public:
		typedef RefCountPtr<T, Allocator>					ThisType;
		typedef T											ValueType;
		typedef Allocator									AllocatorType;

		AllocatorType m_allocator;
		ValueType	m_value;

		RefCountPtr(ValueType value, AllocatorType allocator)
			: RefCount(), m_value(value), m_allocator(std::move(allocator))
		{}

		FORCEINLINE void FreeValue()
		{
			Memory::DestroyItems(m_value, 1);
			m_allocator.Free(m_value);
			m_value = nullptr;
		}

		FORCEINLINE void FreeRefCount()
		{
			AllocatorType allocator = m_allocator;
			this->~ThisType();
			allocator.Free(this);
		}
	};

	template<typename T, typename Allocator>
	class RefCountInstance : public RefCount
	{
	public:
		typedef RefCountInstance<T, Allocator>							ThisType;
		typedef T														ValueType;
		typedef Allocator												AllocatorType;
		typedef typename AlignedStorage<sizeof(T), alignof(T)>::Type	StorageType;

		StorageType m_storage;
		AllocatorType m_allocator;

		template <typename... Args>
		RefCountInstance(AllocatorType allocator, Args&&... args)
			: RefCount(), m_allocator(std::move(allocator))
		{
			new ((void*)&m_storage) ValueType(std::forward<Args>(args)...);
		}

		ValueType* GetValue()
		{
			return static_cast<ValueType*>(static_cast<void*>(&m_storage));
		}

		void FreeValue()
		{
			GetValue()->~ValueType();
		}

		void FreeRefCount()
		{
			AllocatorType allocator = m_allocator;
			this->~RefCountInstance();
			allocator.Free(this);
		}
	};

	template <typename T, typename Allocator = Allocators::DefaultAllocator>
	class SharedRef
	{
	public:
		typedef SharedRef<T, Allocator>									ThisType;
		typedef T														ElementType;
		typedef Allocator												AllocatorType;
		typedef typename TypeTraits::SharedRefTraits<T>::ReferenceType	ReferenceType;

	protected:
		ElementType* m_pointer;
		RefCount* m_refCounter;

	public:
		SharedRef() noexcept
			:m_pointer(nullptr), m_refCounter(nullptr) 
		{}

		SharedRef(NULLPTRTYPE) noexcept
			:m_pointer(nullptr), m_refCounter(nullptr)
		{}

		template <typename U>
		explicit SharedRef(U* pointer, typename std::enable_if<std::is_convertible<U*, ElementType*>::value, U>::type* = 0) 
		{
			AllocateInternal(pointer);
		}

		SharedRef(const SharedRef& other) noexcept
			: m_pointer(other.m_pointer), m_refCounter(other.m_refCounter)
		{
			if (m_refCounter)
			{
				m_refCounter->AddRef();
			}
		}

		template<typename U>
		SharedRef(const SharedRef<U>& other, typename std::enable_if<std::is_convertible<U*, ElementType*>::value, U>::type* = 0)
			: m_pointer(other.m_pointer), m_refCounter(other.m_refCounter)
		{
			if (m_refCounter)
			{
				m_refCounter->AddRef();
			}
		}

		SharedRef(SharedRef&& other)
			: m_pointer(other.m_pointer), m_refCounter(other.m_refCounter)
		{
			other.m_pointer = nullptr;
			other.m_refCounter = nullptr;
		}

		template<typename U>
		SharedRef(SharedRef<U>&& other, typename std::enable_if<std::is_convertible<U*, ElementType*>::value, U>::type* = 0) noexcept
			: m_pointer(other.m_pointer), m_refCounter(other.m_refCounter)
		{
			other.m_pointer = nullptr;
			other.m_refCounter = nullptr;
		}

		~SharedRef()
		{
			if (m_refCounter)
			{
				m_refCounter->Relese();
			}
			m_pointer = nullptr;
			m_refCounter = nullptr;
		}

		SharedRef& operator=(const SharedRef& other) noexcept
		{
			if (&other != this)
			{
				ThisType(other).Swap(*this);
			}
			return *this;
		}

		template <typename U>
		typename std::enable_if<std::is_convertible<U*, ElementType*>::value, ThisType&>::type 
			operator=(const SharedRef<U>& other) noexcept
		{
			if (!IsOwnershipEquivalent(other))
			{
				ThisType(other).Swap(*this);
			}
			return *this;
		}

		SharedRef& operator=(SharedRef&& other) noexcept
		{
			if (&other != this)
			{
				ThisType(std::move(other)).Swap(*this);
			}
			return *this;
		}

		template <typename U>
		typename std::enable_if<std::is_convertible<U*, ElementType*>::value, ThisType&>::type
			operator=(SharedRef<U>&& other) noexcept
		{
			if (!IsOwnershipEquivalent(other))
			{
				ThisType(std::move(other)).Swap(*this);
			}
			return *this;
		}

		void Swap(SharedRef& other) noexcept
		{
			ElementType* const otherPointer	= other.m_pointer;
			other.m_pointer					= m_pointer;
			m_pointer						= otherPointer;

			RefCount* const otherRefCount	= other.m_refCounter;
			other.m_refCounter				= m_refCounter;
			m_refCounter					= otherRefCount;
		}

		void Reset() noexcept
		{
			ThisType().Swap(*this);
		}

		ReferenceType operator*() const noexcept
		{
			return *m_pointer;
		}

		ElementType* operator->() const noexcept
		{
			return m_pointer;
		}

		ElementType* Get() const noexcept
		{
			return m_pointer;
		}

		int32 UseCount() const noexcept
		{
			return (m_refCounter) ? m_refCounter->GetCount() : 0;
		}

		bool Unique() const noexcept
		{
			return (m_refCounter && (m_refCounter->m_refCount == 1));
		}

		explicit operator bool() const noexcept
		{
			return m_pointer != nullptr;
		}

		template <typename U>
		bool IsOwnershipEquivalent(const SharedRef<U>& sharedRef)
		{
			return (m_refCounter == sharedRef.m_refCounter);
		}

	protected:
		// Friend declarations.
		template <typename U> friend void AllocateSharedRefHelper(SharedRef<U>&, RefCount*, U*);

		template <typename U>
		void AllocateInternal(U* pointer)
		{
			AllocatorType allocator;
			typedef RefCountPtr<U*, AllocatorType> RefCountType;
			void* memory = allocator.Allocate(sizeof(RefCountType));
			T_ASSERT(memory != nullptr);
			m_refCounter = new (memory) RefCountType(pointer, allocator);
			m_pointer = pointer;
		}
	};

	template <typename T, typename U>
	inline bool operator==(const SharedRef<T>& a, const SharedRef<U>& b) noexcept
	{
		return (a.Get() == b.Get());
	}

	template <typename T, typename U>
	inline bool operator!=(const SharedRef<T>& a, const SharedRef<U>& b) noexcept
	{
		return (a.Get() != b.Get());
	}

	template <typename T>
	inline bool operator==(const SharedRef<T>& a, NULLPTRTYPE) noexcept
	{
		return !a;
	}

	template <typename T>
	inline bool operator==(NULLPTRTYPE, const SharedRef<T>& b) noexcept
	{
		return !b;
	}

	template <typename T>
	inline bool operator!=(const SharedRef<T>& a, NULLPTRTYPE) noexcept
	{
		return static_cast<bool>(a);
	}

	template <typename T>
	inline bool operator!=(NULLPTRTYPE, const SharedRef<T>& b) noexcept
	{
		return static_cast<bool>(b);
	}

	template <typename T>
	void AllocateSharedRefHelper(Thor::SharedRef<T>& sharedPointer, RefCount* refCounter, T* value)
	{
		sharedPointer.m_refCounter = refCounter;
		sharedPointer.m_pointer = value;
	}

	template <typename T, typename... Args>
	SharedRef<T> MakeShared(Args&&... args)
	{
		typedef Allocators::DefaultAllocator AllocatorType;
		typedef RefCountInstance<T, AllocatorType> RefCountType;
		AllocatorType allocator;
		SharedRef<T> ret;
		RefCountType* refCount = allocator.New<RefCountType>(allocator, std::forward<Args>(args)...);
		AllocateSharedRefHelper(ret, refCount, refCount->GetValue());
		return ret;
	}
}
#endif // THOR_SMART_POINTERS_H_

