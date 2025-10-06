/**
 * Program: Simple Smart Pointer Implementation
 * Problem Statement: Implement a basic smart pointer class to demonstrate 
 * RAII (Resource Acquisition Is Initialization) in C++
 * 
 * Input Example:
 *   SmartPtr<int> ptr = make_smart<int>(42);
 *   cout << *ptr << endl;
 * 
 * Output Example:
 *   42
 *   Memory automatically released when ptr goes out of scope
 * 
 * Complexity: O(1) for all operations
 * Features: Automatic memory management, Move semantics, Reference counting
 */

#include <iostream>
#include <utility>

template<typename T>
class SmartPtr {
private:
    T* ptr;
    size_t* ref_count;

public:
    // Constructor
    explicit SmartPtr(T* p = nullptr) : ptr(p), ref_count(new size_t(1)) {
        if (ptr) {
            std::cout << "SmartPtr created with value: " << *ptr << std::endl;
        }
    }

    // Copy constructor
    SmartPtr(const SmartPtr& other) : ptr(other.ptr), ref_count(other.ref_count) {
        ++(*ref_count);
        std::cout << "SmartPtr copied, ref count: " << *ref_count << std::endl;
    }

    // Move constructor
    SmartPtr(SmartPtr&& other) noexcept : ptr(other.ptr), ref_count(other.ref_count) {
        other.ptr = nullptr;
        other.ref_count = nullptr;
        std::cout << "SmartPtr moved" << std::endl;
    }

    // Destructor
    ~SmartPtr() {
        if (ref_count && --(*ref_count) == 0) {
            delete ptr;
            delete ref_count;
            std::cout << "SmartPtr destroyed, memory released" << std::endl;
        }
    }

    // Assignment operator
    SmartPtr& operator=(const SmartPtr& other) {
        if (this != &other) {
            // Decrease current ref count
            if (ref_count && --(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }
            // Copy new values
            ptr = other.ptr;
            ref_count = other.ref_count;
            ++(*ref_count);
        }
        return *this;
    }

    // Dereference operators
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    // Get raw pointer
    T* get() const { return ptr; }

    // Check if valid
    bool is_valid() const { return ptr != nullptr; }

    // Get reference count
    size_t use_count() const { return ref_count ? *ref_count : 0; }
};

// Helper function to create SmartPtr
template<typename T, typename... Args>
SmartPtr<T> make_smart(Args&&... args) {
    return SmartPtr<T>(new T(std::forward<Args>(args)...));
}

// Demo function
void demonstrateSmartPtr() {
    std::cout << "\n=== Smart Pointer Demo ===" << std::endl;
    
    // Create smart pointer
    SmartPtr<int> ptr1 = make_smart<int>(100);
    std::cout << "ptr1 value: " << *ptr1 << ", ref count: " << ptr1.use_count() << std::endl;
    
    // Copy smart pointer
    SmartPtr<int> ptr2 = ptr1;
    std::cout << "ptr2 value: " << *ptr2 << ", ref count: " << ptr2.use_count() << std::endl;
    
    // Move smart pointer
    SmartPtr<int> ptr3 = std::move(ptr1);
    std::cout << "ptr3 value: " << *ptr3 << ", ref count: " << ptr3.use_count() << std::endl;
    
    std::cout << "=== End of scope ===" << std::endl;
}

int main() {
    demonstrateSmartPtr();
    std::cout << "Program ended" << std::endl;
    return 0;
}
