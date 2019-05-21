#pragma once

#include <type_traits>
#include <utility>

// Enable empty base class optimization with multiple inheritance on Visual Studio.
#if defined(_MSC_VER) && _MSC_VER >= 1910
#  define FLUENT_EBCO __declspec(empty_bases)
#else
#  define FLUENT_EBCO
#endif


template <typename T>
using IsNotReference = typename std::enable_if<!std::is_reference<T>::value, void>::type;


template <typename T, typename Parameter, template<typename> class... Skills>
class FLUENT_EBCO NamedType : public Skills<NamedType<T, Parameter, Skills...>>...
{
public:
	using UnderlyingType = T;

	// constructor
	explicit constexpr NamedType(T const & value) : value_(value) {}

	template <typename T_ = T, typename = IsNotReference<T_>>
	explicit constexpr NamedType(T && value) : value_(std::move(value)) {}

	// get
	constexpr T & get() { return value_; }
	constexpr std::remove_reference_t<T> const & get() const { return value_; }

	// conversions
	using ref = NamedType<T&, Parameter, Skills...>;

	operator ref()
	{
		return ref(value_);
	}

	struct argument
	{
		template <typename U>
		NamedType operator=(U && value) const
		{
			return NamedType(std::forward<U>(value));
		}

		argument() = default;
		argument(argument const &) = delete;
		argument(argument &&) = delete;
		argument & operator=(argument const &) = delete;
		argument & operator=(argument &&) = delete;
	};


private:
	T value_;
};


template <template<typename T> class StrongType, typename T>
constexpr StrongType<T> make_named(T const & value)
{
	return StrongType<T>(value);
}


template <typename T, typename... Args>
T make_named(Args &&... args)
{
	return T(T::UnderlyingType(std::forward<Args>(args)...));
}