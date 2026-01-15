#pragma once

//-----------------------------------------------------------------------------
// 事件基类
// - 仅包含 type 和 param，可在派生类中扩展更多字段
//-----------------------------------------------------------------------------
class ObservedEvent
{
public:
    const int   m_type;
    int   m_param = 0;

public:
    ObservedEvent(int event_type) : m_type(event_type) {}
    virtual ~ObservedEvent() = default;
};

//-----------------------------------------------------------------------------
// - 因为有static inline变量，检查Fire调用时机
//-----------------------------------------------------------------------------
class IEventObserverBase
{
private:
    static inline std::vector<IEventObserverBase*>   g_list;
    bool   m_observer_registered = true;

public:
    IEventObserverBase()
    {
        g_list.push_back(this);
    }

    virtual ~IEventObserverBase()
    {
        UnregisterObserver();
    }

    // Unregister the observer; call manually for global/static objects to avoid dead references
    void UnregisterObserver()
    {
        if (m_observer_registered)
        {
            std::erase(g_list, this);
            m_observer_registered = false;
        }
    }

    // Fire an event using enum class or integer type
    template<typename E1, typename E2 = int>
    static void FireEvent(E1 event_type, E2 event_param = 0)
    {
        ObservedEvent   t((int)event_type);
        t.m_param = (int)event_param;
        BroadcastEvent(t);
    }

    static void BroadcastEvent(ObservedEvent& event)
    {
        for (auto& iter : g_list)
        {
            iter->OnObserveEvent(event);
        }
    }

protected:
    virtual void OnObserveEvent(ObservedEvent& event) = 0;
};
