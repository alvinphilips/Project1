#pragma once

#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "json.hpp"

template <typename T>
void load_from_json(T& value, const json::JSON& node);

template <>
inline void load_from_json(int& value, const json::JSON& node)
{
	value = node.ToInt();
}

template <>
inline void load_from_json(bool& value, const json::JSON& node)
{
	value = node.ToBool();
}

template <>
inline void load_from_json(std::string& value, const json::JSON& node)
{
	value = node.ToString();
}

template <>
inline void load_from_json(float& value, const json::JSON& node)
{
	value = (float)node.ToFloat();
}

template <>
inline void load_from_json(Uint8& value, const json::JSON& node)
{
	value = (Uint8)node.ToInt();
}

template <>
inline void load_from_json(SDL_Color& value, const json::JSON& node)
{
	value.r = (Uint8)node.at(0).ToInt();
	value.g = (Uint8)node.at(1).ToInt();
	value.b = (Uint8)node.at(2).ToInt();
	value.a = (Uint8)node.at(3).ToInt();
}


#endif