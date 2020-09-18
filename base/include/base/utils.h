#pragma once

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>;

template<typename... T>
struct TypeList
{};


template <template<class...> class TypeList, typename T, typename ...Rest>
void registerToMeta(const TypeList<T, Rest...>& t)
{
	std::string_view name = entt::type_info<decltype(std::declval<T>())>::name();

	// resolve name
	std::string_view delims = " ";
	std::string_view refToken = "&";
	size_t start = 0;
	size_t end = name.size();
	start = name.find_first_of(delims, start) + 1;
	end = name.find_first_of(refToken, start);
	name = name.substr(start, end - start);

	std::string nameStr(name.begin(), name.end());
	std::string execName = nameStr + "::exec";

	entt::hashed_string execNameHash{ execName.c_str() };
	entt::hashed_string nameHash{ nameStr.c_str() };

	entt::meta<T>().type(nameHash).func<&T::exec>(execNameHash);

	if constexpr (sizeof...(Rest) > 0)
	{
		registerToMeta(TypeList<Rest...>());
	}
}