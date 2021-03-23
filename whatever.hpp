#pragma once
#include <type_traits>
#include <initializer_list>
#include <typeinfo>
#include <memory>

namespace utils {

    class whatever {

    private:

        class I_whatever {
        public:
            virtual ~I_whatever(){}
            virtual I_whatever* Copy() const = 0;
            virtual const std::type_info& cur_type() const = 0;
        };

        template<typename T>
        class _whatever final : public I_whatever {
        public:
            T value_;

            explicit _whatever(const T& value) : value_(value) {}

            I_whatever* Copy() const override {
                return new _whatever<T>(value_);
            }

            const std::type_info& cur_type() const override {
                return typeid(value_);
            }
        };

        I_whatever* ptr_ = nullptr;

    public:

        whatever() : ptr_(nullptr) {}

        whatever(std::nullptr_t x) : ptr_(x) {}

        template<typename T>
        whatever(const T& value) : ptr_(new _whatever<typename std::decay<const T>::type>(value)) {}

        template<typename T>
        whatever(T& value) : ptr_(new _whatever<typename std::decay<T>::type>(value)) {}

        template<typename T>
        whatever& operator=(const T& x) {
            delete ptr_;
            ptr_ = new _whatever<typename std::decay<const T>::type>(x);
            return *this;
        }

        template<typename T>
        whatever& operator=(T& x) {
            delete ptr_;
            ptr_ = new _whatever<typename std::decay<T>::type>(x);
            return *this;
        }

        whatever(const whatever& copy) : ptr_(copy.ptr_ == nullptr ? nullptr : copy.ptr_->Copy()) {}
        whatever(whatever& copy) : ptr_(copy.ptr_ == nullptr ? nullptr : copy.ptr_->Copy()) {}
        whatever& operator=(whatever& copy) {
            if (this == &copy) {
                return *this;
            }
            delete ptr_;
            if (copy.ptr_ != nullptr) {
                ptr_ = copy.ptr_->Copy();
            } else {
                ptr_ = nullptr;
            }
            return *this;
        }
        whatever& operator=(const whatever& copy) {
            if (this == &copy) {
                return *this;
            }
            delete ptr_;
            if (copy.ptr_ != nullptr) {
                ptr_ = copy.ptr_->Copy();
            } else {
                ptr_ = nullptr;
            }
            return *this;
        }

        ~whatever() {
            if (ptr_ != nullptr) {
                delete ptr_;
                ptr_ = nullptr;
            }
        }

        void clear() {
            delete ptr_;
            ptr_ = nullptr;
        }

        bool empty() const {
            return ptr_ == nullptr;
        }

        const std::type_info& cur_type() const {
            return ptr_ == nullptr ? typeid(void) : ptr_->cur_type();
        }

        template<typename T> friend T* whatever_cast(whatever* cur);
        template<typename T> const T* whatever_cast(const whatever* cur);
        template<typename T> T whatever_cast(whatever& cur);
        template<typename T> T whatever_cast(const whatever& cur);

        friend void swap(whatever& a, whatever& b);
    };

    class bad_whatever_cast : public std::bad_cast {
    public:
        const char* what() const noexcept override {
            return "utils::bad_whatever_cast: " "smth go wrong";
        }
    };

    template<typename T>
    inline T* whatever_cast(whatever* cur) {
        if (cur == nullptr) {
            return nullptr;
        }
        if (cur->cur_type() == typeid(T)) {
            return &static_cast<whatever::_whatever<T> *>(cur->ptr_)->value_;
        }

        return nullptr;
    }

    template<typename T>
    inline const T* whatever_cast(const whatever* cur) {
        if (cur == nullptr) {
            return nullptr;
        }

        return whatever_cast<T>(const_cast<whatever*>(cur));
    }

    template<typename T>
    inline T whatever_cast(whatever& cur) {
        if (!whatever_cast<typename std::remove_reference<T>::type>(&cur)) {
            throw bad_whatever_cast();
        }

        return *whatever_cast<typename std::remove_reference<T>::type>(&cur);
    }

    template<typename T>
    inline T whatever_cast(const whatever& cur) {
        return whatever_cast<const typename std::remove_reference<T>::type&>(const_cast<whatever&>(cur));
    }

    inline void swap(whatever& a, whatever& b) {
        std::swap(a.ptr_, b.ptr_);
    }
}