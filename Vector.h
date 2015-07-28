#include <memory>
#include <algorithm>

template<class T>
class Vector
{
public:

	typedef T* iterator;
	typedef const T* const_iterator;

	Vector()
		: vectorData(0), lastVectorElement(0), lastAllocatedElement(0)
	{}

	Vector(size_t numberOfElements, T value = T())
	{
		createVector(numberOfElements, value);
	}

	Vector(Vector& vec)
	{
		createVector(vec.vectorData, vec.lastVectorElement);
	}

	Vector& operator=(Vector& vec)
	{
		if (&vec != this)
		{
			createVector(vec.vectorData, vec.lastVectorElement);
		}

		return *this;
	}

	~Vector()
	{
		destroyVector();
	}

	size_t size() const
	{
		return lastVectorElement - vectorData;
	}
	
	T& operator[](size_t index)
	{
		return vectorData[index];
	}

	const T& operator[](size_t index) const
	{
		return vectorData[index];
	}

	void push_back(T newElement)
	{
		if (lastVectorElement >= lastAllocatedElement)
			growVector();

		appendNewElementToVector(newElement);
	}

	void pop_back()
	{
		removeElementFromVector();
	}

	iterator begin()
	{
		return vectorData;
	}

	const_iterator cbegin() const
	{
		return vectorData;
	}

	iterator end()
	{
		return lastVectorElement;
	}

	const_iterator cend() const
	{
		return lastVectorElement;
	}

private:

	std::allocator<T> allocator;

	void createVector(size_t numberOfElements, T value)
	{
		vectorData = allocator.allocate(numberOfElements);
		lastVectorElement = lastAllocatedElement = vectorData + numberOfElements;
		std::uninitialized_fill(vectorData, lastVectorElement, value);
	}

	void createVector(iterator begin, iterator end)
	{
		size_t numberOfCreatedElements = end - begin;
		vectorData = allocator.allocate(numberOfCreatedElements);
		lastVectorElement = std::uninitialized_copy(begin, end, vectorData);
		lastAllocatedElement = lastVectorElement;
	}

	void growVector()
	{
		size_t allocatedElements = (lastAllocatedElement - vectorData) * 2;
		size_t newAllocatedElementsSize = std::max(allocatedElements, size_t(1));
		T* newVectorData = allocator.allocate(newAllocatedElementsSize);
		lastVectorElement = std::uninitialized_copy(vectorData, lastVectorElement, newVectorData);
		vectorData = newVectorData;
		lastAllocatedElement = vectorData + newAllocatedElementsSize;
	}

	void appendNewElementToVector(T newElement)
	{
		allocator.construct(lastVectorElement++, newElement);
	}

	void removeElementFromVector()
	{
		allocator.destroy(--lastVectorElement);
	}

	void destroyVector()
	{
		while (vectorData >= lastVectorElement)
		{
			allocator.destroy(--lastVectorElement);
		}

		allocator.deallocate(vectorData, (lastAllocatedElement - vectorData));
	}

	T* vectorData;
	T* lastVectorElement;
	T* lastAllocatedElement;
};