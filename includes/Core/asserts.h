#pragma once

#define GET_LAST_GRAPHICS_ERROR(hr)                                                         \
    {                                                                                       \
        std::stringstream ss;                                                               \
        auto sErrMsg = (hr == DXGI_ERROR_DEVICE_REMOVED) ?                                  \
            m_spDevice->GetDeviceRemovedReason() : S_OK;                                    \
                                                                                            \
        ss <<  __FILE__ << ": " << __LINE__ << std::endl <<                                 \
            sErrMsg << std::endl;                                                           \
        MessageBox(nullptr, reinterpret_cast<LPCSTR>(ss.str().c_str()), nullptr,            \
            MB_OK|MB_ICONERROR);                                                            \
        PostQuitMessage(hr);                                                                \
    }

#define LOG_SHADER_COMPILE_ERROR(spErrorBuffer)                                             \
    {                                                                                       \
        std::stringstream ss;                                                               \
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());                                    \
        _com_error err(hr);                                                                 \
        const char* sErrMsg = (spErrorBuffer) ?                                             \
            (const char*)spErrorBuffer->GetBufferPointer() : "";                            \
                                                                                            \
        ss <<  __FILE__ << ": " << __LINE__ << std::endl <<                                 \
            "Win Error: " << err.ErrorMessage() << std::endl <<                             \
            "Compile Error: " << sErrMsg << std::endl;                                      \
        MessageBox(nullptr, reinterpret_cast<LPCSTR>(ss.str().c_str()), nullptr,            \
            MB_OK|MB_ICONERROR);                                                            \
    }

#define LOG_LAST_ERROR()                                                                    \
    {                                                                                       \
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());                                    \
        std::stringstream ss;                                                               \
        _com_error err(hr);                                                                 \
        ss <<  __FILE__ << ": " << __LINE__ << std::endl <<                                 \
            err.ErrorMessage() << std::endl;                                                \
        MessageBox(nullptr, reinterpret_cast<LPCSTR>(ss.str().c_str()), nullptr,            \
            MB_OK|MB_ICONERROR);                                                            \
        PostQuitMessage(hr);                                                                \
    }

#define LOG_ERROR(hr)                                                                       \
    {                                                                                       \
        std::stringstream ss;                                                               \
        _com_error err(hr);                                                                 \
        ss <<  __FILE__ << ": " << __LINE__ << std::endl <<                                 \
            err.ErrorMessage() << std::endl;                                                \
        MessageBox(nullptr, reinterpret_cast<LPCSTR>(ss.str().c_str()), nullptr,            \
            MB_OK|MB_ICONERROR);                                                            \
        PostQuitMessage(hr);                                                                \
    }