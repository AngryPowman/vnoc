#pragma once
//////////////////////////////////////////////////////////////////////////
// ����C++09�����߰汾��bindʵ��

template<typename R, typename... Args>
struct invoker_base {
	virtual R invoke(Args...) = 0;
	virtual ~invoker_base() { }
};

template<typename F, typename R, typename... Args>
struct functor_invoker : public invoker_base<R, Args...>
{
	explicit functor_invoker(F f) : f(f) { }
	R invoke(Args... args) { return f(args...); }
private:
	F f;
};

template<typename Signature>
class function;

template<typename R, typename... Args>
class function<R (Args...)>
{
public:
	template<typename F>
	function(F f) : invoker(0)
	{
		invoker = new functor_invoker<F, R, Args...>(f);
	}

	R operator()(Args... args) const
	{
		return invoker->invoke(args...);
	}

private:
	invoker_base<R, Args...>* invoker;
};