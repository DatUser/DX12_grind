#pragma once

#include <chrono>

#include "window.h"

/**
 * @brief Singleton class representing engine app
 *
 */
class App
{
	friend std::unique_ptr<App>::deleter_type;

public:

    int Execute();
    void RetrieveFrame();

    inline double GetExecutionTime();
	inline Window* GetMainWindow();
    //inline RHI* GetRHI();

    inline static auto&& GetInstance();

private:
	App() = delete;
    App(LPCSTR pWinName);
    ~App();

	static std::unique_ptr<App> m_spAppInstance;

    Window* m_pMainWindow;
    std::chrono::time_point<std::chrono::steady_clock> m_oStartTime;
};

#include "app.hpp"