// polymorfism_ext.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#include <boost/python.hpp>

class CppBase {
public:
	CppBase() { std::cerr << __FUNCTION__ << std::endl;}
	virtual ~CppBase() = 0 { std::cerr << __FUNCTION__ << std::endl; }
	virtual std::string hello() = 0;
private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version) { }
};

class CppDerived : public CppBase {
public:
	CppDerived() { std::cerr << __FUNCTION__ << std::endl; }
	CppDerived(std::string const& data) : data_(data) { std::cerr << __FUNCTION__ << std::endl; }
	virtual ~CppDerived() { std::cerr << __FUNCTION__ << std::endl; }
	virtual std::string hello() { return "Cpp: " + data_; }

	std::string data_;

private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & boost::serialization::base_object<CppBase>(*this);
		ar & data_;
	}
};

template<typename T>
struct serialize_pickle
	: boost::python::pickle_suite
{

	static boost::python::tuple getstate(const T& item)
	{

		const char* tpn = typeid(T).name();
		std::ostringstream os;
		boost::archive::text_oarchive oa(os);
		oa << item;

		return boost::python::make_tuple(os.str());
	}

	static void setstate(T& item, boost::python::tuple state)
	{
		const char* tpn = typeid(T).name();
		using namespace boost::python;
		str data = extract<str>(state[0]);
		std::string temp(extract<const char*>(data), len(data));
		std::istringstream is(temp);
		boost::archive::text_iarchive ia(is);
		ia >> item;
		const char* tpn2 = typeid(T).name();

		int igfasg = 0;
	}

};

BOOST_CLASS_EXPORT(CppDerived)


void print_hello(CppBase* p) {
	std::cerr << __FUNCTION__ << std::endl;
	std::cerr << typeid(p).name() << std::endl;
	std::cerr << typeid(*p).name() << std::endl;
	std::cout << "print_hello" << p->hello() << std::endl;
}

void print_hello2(boost::shared_ptr<CppBase> p) {
	std::cerr << __FUNCTION__ << std::endl;
	std::cerr << typeid(p).name() << std::endl;
	std::cerr << typeid(*p).name() << std::endl;
	std::cout << "print_hello" << p->hello() << std::endl;
}


BOOST_PYTHON_MODULE_INIT(polymorfism_ext)
{
	using namespace boost::python;
	
	struct CppBase_wrap : CppBase, wrapper < CppBase > {
		std::string hello() { return this->get_override("hello")(); }
	};

	class_<CppBase_wrap, boost::noncopyable>("CppBase")
		.def("hello", pure_virtual(&CppBase::hello))
		.enable_pickling()
		;

	register_ptr_to_python<boost::shared_ptr<CppBase>>();

	class_<CppDerived, bases<CppBase>>("CppDerived", init<>())
		.def(init<std::string const&>(args("data")))
		.def_readwrite("data_", &CppDerived::data_)
		.def_pickle(serialize_pickle<CppDerived>())
		;

	def("print_hello", print_hello, args("CppBase_RawPtr"));
	def("print_hello2", print_hello2, args("CppBase_SharedPtr"));
}