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
// - 禁止全局/静态类继承，防止dead reference
//-----------------------------------------------------------------------------
class IEventObserverBase
{
private:
    static inline std::vector<IEventObserverBase*>   g_list;

public:
    IEventObserverBase()
    {
        g_list.push_back(this);
    }

    virtual ~IEventObserverBase()
    {
        std::erase(g_list, this);
    }

    // 支持 enum class 的模板版本
    template<class E1, class E2 = int>
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
