#pragma once

#include <new>
#include <unordered_map>
#include <nlohmann/json.hpp>

/* Functor needs to have : json iterator pointer */
template <typename Functor, bool ParseSub = true> class json_loader
{
private:
	std::unordered_map<std::string, Functor *> map_funcs;

	nlohmann::json json_handle;

	typename Functor::data_type data;
public:
	template <typename T> struct functor_type
	{
		using type = T;
		std::string key;
	};

	/* to initialize functors, need to use the template type "functor_type<T>" with T as all the derived classes of the base functor */
	template <typename ... FunctorKeys> auto init_functors(FunctorKeys && ... inits) -> void
	{
		auto init_func = [this](auto f) { map_funcs[f.key] = new typename decltype(f)::type; };

		(init_func(inits), ...);
	}

	auto init_data(typename Functor::data_type const & data) -> void
	{
		this->data = data;
	}

	auto load(std::string const & json_contents
		, std::function<void(typename Functor::data_type &)> const & func) -> void
	{
		json_handle = nlohmann::json::parse(json_contents);

		data.handle = &json_handle;

		for (nlohmann::json::iterator it = json_handle.begin(); it != json_handle.end(); ++it)
		{
			data.iterator = &it;

			func(data);

			if constexpr (ParseSub) parse_sub(data, it);
		}
	}
private:
	auto parse_sub(typename Functor::data_type & data, nlohmann::json::iterator & it) -> void
	{
		for (nlohmann::json::iterator sub_it = it.value().begin(); sub_it != it.value().end(); ++sub_it)
		{
			data.iterator = &sub_it;
			map_funcs[sub_it.key()]->apply(data);
		}
	}
};