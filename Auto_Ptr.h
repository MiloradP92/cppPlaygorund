template<class T>
class auto_ptr
{
public:

	auto_ptr()
		: m_pointee(0)
	{}

	auto_ptr(T* pointer)
		: m_pointee(pointer)
	{}

	auto_ptr(auto_ptr& autoPointer)
	{
		copyAndReleasePointer(autoPointer);
	}

	auto_ptr& operator=(auto_ptr& autoPointer)
	{
		if (&autoPointer != this)
		{
			copyAndReleasePointer(autoPointer);
		}

		return *this;
	}

	~auto_ptr() { delete m_pointee; }

	T* get()
	{
		return m_pointee;
	}

	T& operator*()
	{
		return *m_pointee;
	}

	T* operator->()
	{
		return get();
	}

	void release()
	{
		m_pointee = 0;
	}

private:

	void copyAndReleasePointer(auto_ptr& autoPointer)
	{
		m_pointee = autoPointer.get();
		autoPointer.release();
	}

	T* m_pointee;
};