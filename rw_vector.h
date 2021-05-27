#pragma once

#ifndef __RW_VECTOR__
#define __RW_VECTOR__ 100

#include "rw_msort.h"

namespace stdrw
{
    enum VectorParameters
    {
        DEFAULT_CAPACITY = 10,
        MINIMUM_CAPACITY = 1,
        MAXIMUM_CAPACITY = 0x7FFFFFFF
    };

    template<typename datatype>
    class Vector
    {
    private:
        // Private variants
        long capacity_;

        // Private static methods
        static bool default_is_bigger(const datatype &a, const datatype &b);

    protected:
        // Protected variants
        datatype *data_;
        long amount_;

        // Protected normal methods
        bool set_capacity(long new_capacity);

        long get_capacity() const;

    public:
        // Constructors and Destructors
        explicit Vector(const datatype *data_to_set = nullptr, long capacity_to_set = DEFAULT_CAPACITY,
                        long amount_to_set = 0);

        Vector(const datatype &init_value, long amount_to_set);

        Vector(Vector<datatype> const &another);

        ~Vector();

        // Static methods
        static void SetArray(datatype *data_array, const datatype &value, long length);

        static void CopyArray(datatype *destination, const datatype *source, long length);

        // Normal methods
        long get_element_amount();

        const datatype &operator[](long index) const;

        datatype &ElementAt(long index);

        bool Append(const datatype &new_element);

        long Append(const datatype *new_elements, long length);

        long Append(const Vector<datatype> &new_elements);

        bool Insert(const datatype &new_element, long index);

        bool Push(const datatype &new_element);

        bool Enqueue(const datatype &new_element);

        Vector<datatype> &operator||(const datatype *new_element);

        Vector<datatype> &operator||(const Vector<datatype> &new_elements);

        datatype &Pop();

        datatype &Dequeue();

        bool DeleteElementAt(long index);

        bool DeleteElementWithValue(const datatype &element_to_delete);

        long DeleteAllElementsWithValue(const datatype &element_to_delete);

        void Sort(int order);

        void Sort(bool (*ShouldBeInFrontOf)(const datatype &a, const datatype &b) = default_is_bigger);

        Vector<datatype> &operator=(const Vector<datatype> &another);

        bool operator==(const Vector<datatype> &another);
    };

} // namespace stdrw

template<typename datatype>
stdrw::Vector<datatype>::Vector(const datatype *data_to_set, long capacity_to_set, long amount_to_set)
{
    capacity_ = (capacity_to_set < VectorParameters::MINIMUM_CAPACITY) ? VectorParameters::DEFAULT_CAPACITY : capacity_to_set;
    amount_ = (amount_to_set < 0 || amount_to_set > capacity_) ? 0 : amount_to_set;
    data_ = new datatype[capacity_];

    if (data_to_set != nullptr)
        CopyArray(data_, data_to_set, amount_);
}

template<typename datatype>
stdrw::Vector<datatype>::Vector(const datatype &init_value, long amount_to_set)
{
    amount_ = (amount_to_set <= 0) ? 1 : amount_to_set;
    capacity_ = (amount_ <= VectorParameters::DEFAULT_CAPACITY) ? VectorParameters::DEFAULT_CAPACITY : amount_;
    data_ = new datatype[capacity_];

    SetArray(data_, init_value, amount_);
}

template<typename datatype>
stdrw::Vector<datatype>::Vector(stdrw::Vector<datatype> const &another)
{
    capacity_ = another.capacity_;
    amount_ = another.amount_;
    data_ = new datatype[capacity_];

    CopyArray(data_, another.data_, amount_);
}

template<typename datatype>
stdrw::Vector<datatype>::~Vector() { delete[] data_; }

template<typename datatype>
long stdrw::Vector<datatype>::get_capacity() const
{
    return capacity_;
}

template<typename datatype>
bool stdrw::Vector<datatype>::set_capacity(long new_capacity)
{
    datatype *new_data;
    if (new_capacity < VectorParameters::MINIMUM_CAPACITY || new_capacity < amount_)
        return false;

    capacity_ = new_capacity;

    new_data = new datatype[capacity_];
    CopyArray(new_data, data_, amount_);
    delete[] data_;

    data_ = new_data;

    return true;
}

template<typename datatype>
long stdrw::Vector<datatype>::get_element_amount()
{
    return amount_;
}

template<typename datatype>
const datatype &stdrw::Vector<datatype>::operator[](long index) const
{
    if (index >= amount_ || index < 0)
        return *(new datatype());
    else
        return data_[index];
}

template<typename datatype>
datatype &stdrw::Vector<datatype>::ElementAt(long index)
{
    if (index >= amount_ || index < 0)
        return *(new datatype());
    else
        return *(new datatype(data_[index]));
}

template<typename datatype>
bool stdrw::Vector<datatype>::Append(const datatype &new_element)
{
    if (amount_ == VectorParameters::MAXIMUM_CAPACITY)
        return false;

    if (++amount_ > capacity_)
        set_capacity((amount_ > (VectorParameters::MAXIMUM_CAPACITY >> 1)) ? VectorParameters::MAXIMUM_CAPACITY : ((amount_ << 1) - 2));

    data_[amount_ - 1] = new_element;

    return true;
}

template<typename datatype>
long stdrw::Vector<datatype>::Append(const datatype *new_elements, long length)
{
    long appended;

    for (appended = 0; appended < length; appended++)
    {
        if (amount_ == VectorParameters::MAXIMUM_CAPACITY)
            return appended;

        if (++amount_ > capacity_)
            set_capacity((amount_ > (VectorParameters::MAXIMUM_CAPACITY >> 1)) ? VectorParameters::MAXIMUM_CAPACITY : ((amount_ << 1) - 2));

        data_[amount_ - 1] = new_elements[appended];
    }
}

template<typename datatype>
long stdrw::Vector<datatype>::Append(const stdrw::Vector<datatype> &new_elements)
{
    long appended;

    for (appended = 0; appended < new_elements.amount_; appended++)
    {
        if (amount_ == VectorParameters::MAXIMUM_CAPACITY)
            return appended;

        if (++amount_ > capacity_)
            set_capacity((amount_ > (VectorParameters::MAXIMUM_CAPACITY >> 1)) ? VectorParameters::MAXIMUM_CAPACITY : ((amount_ << 1) - 2));

        data_[amount_ - 1] = new_elements.data_[appended];
    }

    return appended;
}

template<typename datatype>
bool stdrw::Vector<datatype>::Insert(const datatype &new_element, long index)
{
    long i;
    if (amount_ == VectorParameters::MAXIMUM_CAPACITY)
        return false;

    if (index > amount_ || index < 0)
        index = amount_;

    if (++amount_ > capacity_)
        set_capacity((amount_ > (VectorParameters::MAXIMUM_CAPACITY >> 1)) ? VectorParameters::MAXIMUM_CAPACITY : ((amount_ << 1) - 2));

    for (i = amount_; i > index; i--)
        data_[i] = data_[i - 1];

    data_[index] = new_element;

    return true;
}

template<typename datatype>
bool stdrw::Vector<datatype>::Push(const datatype &new_element)
{
    if (amount_ == VectorParameters::MAXIMUM_CAPACITY || ++amount_ > capacity_)
        return false;

    data_[amount_ - 1] = new_element;

    return true;
}

template<typename datatype>
bool stdrw::Vector<datatype>::Enqueue(const datatype &new_element)
{
    if (amount_ == VectorParameters::MAXIMUM_CAPACITY || ++amount_ > capacity_)
        return false;

    data_[amount_ - 1] = new_element;

    return true;
}

template<typename datatype>
stdrw::Vector<datatype> &stdrw::Vector<datatype>::operator||(const datatype *new_element)
{
    auto *temp = new stdrw::Vector<datatype>(*this);
    if (temp->amount_ == VectorParameters::MAXIMUM_CAPACITY)
        return *temp;

    if ((temp->amount_ = temp->amount_ + 1) > temp->capacity_)
        temp->set_capacity((temp->amount_ > (VectorParameters::MAXIMUM_CAPACITY >> 1)) ? VectorParameters::MAXIMUM_CAPACITY : ((temp->amount_ << 1) - 2));

    temp->data_[temp->amount_ - 1] = *new_element;

    return *temp;
}

template<typename datatype>
stdrw::Vector<datatype> &stdrw::Vector<datatype>::operator||(const stdrw::Vector<datatype> &new_elements)
{
    auto *temp = new stdrw::Vector<datatype>(*this);
    long i;

    for (i = 0; i < new_elements.amount_; i++)
    {
        if (temp->amount_ == VectorParameters::MAXIMUM_CAPACITY)
            return *temp;

        if ((temp->amount_ = temp->amount_ + 1) > temp->capacity_)
            temp->set_capacity((temp->amount_ > (VectorParameters::MAXIMUM_CAPACITY >> 1)) ? VectorParameters::MAXIMUM_CAPACITY : ((temp->amount_ << 1) - 2));

        temp->data_[temp->amount_ - 1] = new_elements.data_[i];
    }

    return *temp;
}

template<typename datatype>
datatype &stdrw::Vector<datatype>::Pop()
{
    if (amount_ == 0)
        return *(new datatype());

    datatype result = data_[amount_ - 1];
    data_[(amount_--) - 1] = datatype();
    if (amount_ < (capacity_ >> 2))
        set_capacity(capacity_ >> 1);

    return *(new datatype(result));
}

template<typename datatype>
datatype &stdrw::Vector<datatype>::Dequeue()
{
    if (amount_ == 0)
        return *(new datatype());

    datatype result = data_[0];
    amount_--;

    CopyArray(&data_[0], &data_[1], amount_);

    if (amount_ < (capacity_ >> 2))
        set_capacity(capacity_ >> 1);

    return *(new datatype(result));
}

template<typename datatype>
bool stdrw::Vector<datatype>::DeleteElementAt(long index)
{
    if (amount_ == 0 || index < 0 || index >= amount_)
        return false;

    amount_--;
    CopyArray(&data_[index], &data_[index + 1], amount_ - index);
    data_[amount_] = datatype();

    if (amount_ < (capacity_ >> 2))
        set_capacity(capacity_ >> 1);

    return true;
}

template<typename datatype>
bool stdrw::Vector<datatype>::DeleteElementWithValue(const datatype &element_to_delete)
{
    long i;
    for (i = 0; i < amount_; i++)
        if (data_[i] == element_to_delete)
        {
            DeleteElementAt(i);
            return true;
        }
    return false;
}

template<typename datatype>
long stdrw::Vector<datatype>::DeleteAllElementsWithValue(const datatype &element_to_delete)
{
    long i, count = 0;
    for (i = 0; i < amount_; i++)
        if (data_[i] == element_to_delete)
        {
            DeleteElementAt(i--);
            count++;
        }

    return count;
}

template<typename datatype>
void stdrw::Vector<datatype>::Sort(int order)
{
    stdrw::MergeSort<datatype>::SortByDefaultCompareOperator(data_, amount_, order);
}

template<typename datatype>
void stdrw::Vector<datatype>::Sort(bool (*ShouldBeInFrontOf)(const datatype &a, const datatype &b))
{
    stdrw::MergeSort<datatype>::SortByGivenCompareFunction(data_, amount_, ShouldBeInFrontOf);
}

template<typename datatype>
stdrw::Vector<datatype> &stdrw::Vector<datatype>::operator=(const stdrw::Vector<datatype> &another)
{
    if (*this == another)
        return *this;

    delete[] this->data_;
    this->capacity_ = another.capacity_;
    this->amount_ = another.amount_;
    this->data_ = new datatype[capacity_];

    CopyArray(data_, another.data_, amount_);

    return *this;
}

template<typename datatype>
bool stdrw::Vector<datatype>::operator==(const stdrw::Vector<datatype> &another)
{
    long i;
    if (amount_ != another.amount_)
        return false;
    for (i = 0; i < amount_; i++)
        if (data_[i] != another.data_[i])
            return false;
    return true;
}

template<typename datatype>
void stdrw::Vector<datatype>::SetArray(datatype *data_array, const datatype &value, long length)
{
    long i;
    for (i = 0; i < length; i++)
        data_array[i] = value;
}

template<typename datatype>
void stdrw::Vector<datatype>::CopyArray(datatype *destination, const datatype *source, long length)
{
    long i;
    for (i = 0; i < length; i++)
        destination[i] = source[i];
}

template<typename datatype>
bool stdrw::Vector<datatype>::default_is_bigger(const datatype &a, const datatype &b)
{
    return a > b;
}

#endif // __RW_VECTOR__