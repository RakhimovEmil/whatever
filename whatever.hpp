#pragma once
#include <type_traits>
#include <typeinfo>
#include <memory>

namespace utils {
    class whatever {
    public:
        whatever() : ptr_(nullptr) {}
        explicit whatever(std::nullptr_t x) : ptr_(x) {}

        template<typename T>
        whatever(const T& value) : ptr_(new _whatever<std::decay_t<const T>>(value)) {}

        template<typename T>
        whatever(T& value) : ptr_(new _whatever<std::decay_t<T>>(value)) {}

        whatever(const whatever& copy) : ptr_(copy.ptr_ == nullptr ? nullptr : copy.ptr_->Copy()) {}
        whatever(whatever& copy)       : ptr_(copy.ptr_ == nullptr ? nullptr : copy.ptr_->Copy()) {}

        template<typename T>
        whatever& operator=(T& x) {
            whatever cur(x);
            this->swap(cur);
            return *this;
        }

        whatever& operator=(const whatever& copy) {
            if (this == &copy) {
                return *this;
            }
            whatever cur(copy);
            this->swap(cur);
            return *this;
        }

        void clear() {
            delete ptr_;
            ptr_ = nullptr;
        }

        bool empty() const { return ptr_ == nullptr; }

        const std::type_info& cur_type() const { return ptr_ == nullptr ? typeid(void) : ptr_->cur_type(); }

        template<typename T> friend T* whatever_cast(whatever* cur);
        template<typename T> const T* whatever_cast(const whatever* cur);
        template<typename T> T whatever_cast(whatever& cur);
        template<typename T> T whatever_cast(const whatever& cur);

        void swap(whatever& a) { std::swap(ptr_, a.ptr_); }

        ~whatever() {
            if (ptr_ != nullptr) {
                delete ptr_;
                ptr_ = nullptr;
            }
        }
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

            I_whatever* Copy() const override { return new _whatever<T>(value_); }

            const std::type_info& cur_type() const override { return typeid(value_); }
        };

        I_whatever* ptr_ = nullptr;
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
        if (!whatever_cast<std::remove_reference_t<T>>(&cur)) {
            throw bad_whatever_cast();
        }
        return *whatever_cast<std::remove_reference_t<T>>(&cur);
    }

    template<typename T>
    inline T whatever_cast(const whatever& cur) {
        return whatever_cast<const std::remove_reference_t<T>&>(const_cast<whatever&>(cur));
    }

    inline void swap(whatever& a, whatever& b) { a.swap(b); }
}
