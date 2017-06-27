
#ifndef NUCLEUS_CONTAINERS_VECTOR_H_
#define NUCLEUS_CONTAINERS_VECTOR_H_

#include "nucleus/algorithms/copy.h"
#include "nucleus/types.h"

namespace nu {

template <class T>
class Vector {
public:
    typedef T* Iterator;
    typedef const T* ConstIterator;

    typedef T ValueType;

    typedef USize SizeType;

public:
    Vector() : m_capacity(0), m_size(0), m_storage(0) {}

    Vector(const Vector<T>& other) : m_capacity(other.m_size), m_size(other.m_size), m_storage(0) {
        if (other.m_storage) {
            allocateCapacity(m_size);
            uninitializedCopy(other.m_storage, other.m_storage + m_size, m_storage);
        }
    }

    // Construct a vector by copying data from a regular array.
    template <class U>
    Vector(const U* data, SizeType size) {
        m_size = size;
        allocateCapacity(size);
        uninitializedCopy(data, data + m_size, m_storage);
    }

    ~Vector() {
        freeStorage(m_storage, m_size);
        m_storage = 0;
        m_capacity = m_size = 0;
    }

    // Appends element to the end of the vector.
    void pushBack(const T& element) {
        if (m_size + 1 <= m_capacity)
            new ((void*)&m_storage[m_size++]) T(element);
        else
            insertAux(end(), &element, &element + 1);
    }

    void pushBack(const Vector<T>& other) {
        if (m_size + other.size() <= m_capacity) {
            uninitializedCopy(other.begin(), other.end(), end());
            m_size += other.size();
        } else {
            insertAux(end(), other.begin(), other.end());
        }
    }

    // Removes the last element of the vector.
    void popBack() {
        assert(m_size > 0);
        m_size--;
        // We also need to destroy the last object properly here.
        m_storage[m_size].~T();
    }

    // Return a reference to the first element of the vector.
    T& front() {
        assert(m_size > 0);
        return m_storage[0];
    }

    // Returns a constant reference to the first element of the vector.
    const T& front() const {
        assert(m_size > 0);
        return m_storage[0];
    }

    // Return a reference to the last element of the vector.
    T& back() {
        assert(m_size > 0);
        return m_storage[m_size - 1];
    }

    // Return a constant reference to the last element of the vector.
    const T& back() const {
        assert(m_size > 0);
        return m_storage[m_size - 1];
    }

    void insertAt(SizeType index, const T& element) {
        assert(index <= m_size);
        insertAux(m_storage + index, &element, &element + 1);
    }

    void insertAt(SizeType index, const Vector<T>& array) {
        assert(index <= m_size);
        insertAux(m_storage + index, array.begin(), array.end());
    }

    // Insert element before pos.
    void insert(Iterator pos, const T& element) { insertAux(pos, &element, &element + 1); }

    T removeAt(SizeType idx) {
        assert(idx < m_size);
        T tmp = m_storage[idx];
        copy(m_storage + idx + 1, m_storage + m_size, m_storage + idx);
        m_size--;
        // We also need to destroy the last object properly here.
        m_storage[m_size].~T();
        return tmp;
    }

    T& operator[](SizeType idx) {
        assert(idx < m_size);
        return m_storage[idx];
    }

    const T& operator[](SizeType idx) const {
        assert(idx < m_size);
        return m_storage[idx];
    }

    Vector<T>& operator=(const Vector<T>& array) {
        if (this == &array)
            return *this;

        freeStorage(m_storage, m_size);
        m_size = array.m_size;
        allocateCapacity(m_size);
        uninitializedCopy(array.m_storage, array.m_storage + m_size, m_storage);

        return *this;
    }

    SizeType size() const { return m_size; }

    void clear() {
        freeStorage(m_storage, m_size);
        m_storage = 0;
        m_size = 0;
        m_capacity = 0;
    }

    Iterator erase(Iterator pos) {
        copy(pos + 1, m_storage + m_size, pos);
        m_size--;
        // We also need to destroy the last object properly here.
        m_storage[m_size].~T();
        return pos;
    }

    bool empty() const { return (m_size == 0); }

    bool operator==(const Vector<T>& other) const {
        if (this == &other)
            return true;
        if (m_size != other.m_size)
            return false;
        for (SizeType i = 0; i < m_size; ++i) {
            if (m_storage[i] != other.m_storage[i])
                return false;
        }
        return true;
    }

    bool operator!=(const Vector<T>& other) const { return !(*this == other); }

    Iterator begin() { return m_storage; }

    Iterator end() { return m_storage + m_size; }

    ConstIterator begin() const { return m_storage; }

    ConstIterator end() const { return m_storage + m_size; }

    void reserve(SizeType newCapacity) {
        if (newCapacity <= m_capacity)
            return;

        T* oldStorage = m_storage;
        allocateCapacity(newCapacity);

        if (oldStorage) {
            // Copy old data
            uninitializedCopy(oldStorage, oldStorage + m_size, m_storage);
            freeStorage(oldStorage, m_size);
        }
    }

    void resize(SizeType newSize) {
        reserve(newSize);
        for (SizeType i = m_size; i < newSize; ++i)
            new ((void*)&m_storage[i]) T();
        m_size = newSize;
    }

    void assign(ConstIterator first, ConstIterator last) {
        resize(distance(first, last));  // FIXME: ineffective?
        T* dst = m_storage;
        while (first != last)
            *dst++ = *first++;
    }

protected:
    static SizeType roundUpCapacity(SizeType capacity) {
        // Round up capacity to the next power of 2;
        // we use a minimal capacity of 8.
        SizeType capa = 8;
        while (capa < capacity)
            capa <<= 1;
        return capa;
    }

    void allocateCapacity(SizeType capacity) {
        m_capacity = capacity;
        if (capacity) {
            m_storage = (T*)malloc(sizeof(T) * capacity);
            if (!m_storage) {
                //                ::error("Common::Vector: failure to allocate
                //                %u bytes",
                //                        capacity * (SizeType) sizeof(T));
            }
        } else {
            m_storage = 0;
        }
    }

    void freeStorage(T* storage, const SizeType elements) {
        for (SizeType i = 0; i < elements; ++i)
            storage[i].~T();
        free(storage);
    }

    /**
     * Insert a range of elements coming from this or another array.
     * Unlike std::vector::insert, this method does not accept
     * arbitrary iterators, mainly because our iterator system is
     * seriously limited and does not distinguish between input iterators,
     * output iterators, forward iterators or random access iterators.
     *
     * So, we simply restrict to Array iterators. Extending this to arbitrary
     * random access iterators would be trivial.
     *
     * Moreover, this method does not handle all cases of inserting a subrange
     * of an array into itself; this is why it is private for now.
     */
    Iterator insertAux(Iterator pos, ConstIterator first, ConstIterator last) {
        assert(m_storage <= pos && pos <= m_storage + m_size);
        assert(first <= last);
        const SizeType n = last - first;
        if (n) {
            const SizeType idx = pos - m_storage;
            if (m_size + n > m_capacity || (m_storage <= first && first <= m_storage + m_size)) {
                T* const oldStorage = m_storage;

                // If there is not enough space, allocate more.
                // Likewise, if this is a self-insert, we allocate new
                // storage to avoid conflicts.
                allocateCapacity(roundUpCapacity(m_size + n));

                // Copy the data from the old storage till the position where
                // we insert new data
                uninitializedCopy(oldStorage, oldStorage + idx, m_storage);
                // Copy the data we insert
                uninitializedCopy(first, last, m_storage + idx);
                // Afterwards copy the old data from the position where we
                // insert.
                uninitializedCopy(oldStorage + idx, oldStorage + m_size, m_storage + idx + n);

                freeStorage(oldStorage, m_size);
            } else if (idx + n <= m_size) {
                // Make room for the new elements by shifting back
                // existing ones.
                // 1. Move a part of the data to the uninitialized area
                uninitializedCopy(m_storage + m_size - n, m_storage + m_size, m_storage + m_size);
                // 2. Move a part of the data to the initialized area
                copy_backward(pos, m_storage + m_size - n, m_storage + m_size);

                // Insert the new elements.
                copy(first, last, pos);
            } else {
                // Copy the old data from the position till the end to the new
                // place.
                uninitializedCopy(pos, m_storage + m_size, m_storage + idx + n);

                // Copy a part of the new data to the position inside the
                // initialized space.
                copy(first, first + (m_size - idx), pos);

                // Copy a part of the new data to the position inside the
                // uninitialized space.
                uninitializedCopy(first + (m_size - idx), last, m_storage + m_size);
            }

            // Finally, update the internal state
            m_size += n;
        }
        return pos;
    }

protected:
    SizeType m_capacity;
    SizeType m_size;
    T* m_storage;
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_VECTOR_H_
