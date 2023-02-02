#pragma once
#include <stdexcept>

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace YunutyEngine
{
    template<typename T>
    class cache
    {
    private:
        T data;
        bool dirty = true;
    public:
        cache() :data(T()) {}
        cache(const T& data) :data(data), dirty(false) {  }
        operator T() const
        {
#if _DEBUG
            if (dirty)
                throw std::runtime_error(std::string("trying to read cahced data while it's dirty!"));
#endif
            return data;
        }
        T& operator=(const T& data) { dirty = false; return this->data = data; }
        T& operator=(const cache& cachedData) { dirty = false; return this->data = cachedData->data; }
        void SetDirty() { dirty = true; }
        bool IsDirty() { return dirty; }
    };
}
