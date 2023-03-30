template<typename RetType, typename... ParamsType>
Event<RetType, ParamsType...>::Event()
:   m_vCallbacks(std::vector<RetType, ParamsType...>())
{}

template<typename RetType, typename... ParamsType>
Event<RetType, ParamsType...>::~Event()
{}

template<typename RetType, typename... ParamsType>
void Event<RetType, ParamsType...>::AddAction(RetType action(ParamsType...))
{
    m_vCallbacks.push_back(action);
}

template<typename RetType, typename... ParamsType>
void Event<RetType, ParamsType...>::Broadcast(ParamsType... params)
{
    //TODO: Use params thread safely
    for (auto&& callback : m_vCallbacks)
        callback(params);
}

template<typename RetType, typename... ParamsType>
void Event<RetType, ParamsType...>::RemoveAction(RetType action(ParamsType...))
{
    m_vCallbacks.erase(action);
}