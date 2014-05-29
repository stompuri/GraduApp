#pragma once

namespace SST
{
	class ParameterBase
	{
	public:
		virtual ~ParameterBase() {}
		template<class T>
		T& get(); //to be implimented after Parameter
		template<class T, class U>
		void setValue(U rhs); //to be implimented after Parameter
	};

	template <typename T>
	class Parameter : public ParameterBase
	{
	public:
		Parameter() :value(0) {}
		Parameter(T rhs) :value(rhs) { }
		T& get() { return value; }
		void setValue(T rhs) { value=rhs; }
	private:
		T value;
	};

	//Here's the trick: dynamic_cast rather than virtual
	template<class T>
	T& ParameterBase::get()
	{
		return dynamic_cast<Parameter<T>*>(this)->get();
	}

	template<class T, class U>
	void ParameterBase::setValue(U rhs)
	{
		dynamic_cast<Parameter<T>*>(this)->setValue(rhs);
	}

}