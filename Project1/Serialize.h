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
inline void load_from_json(unsigned int& value, const json::JSON& node)
{
	value = (unsigned int)node.ToInt();
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
inline void load_from_json(SDL_FPoint& value, const json::JSON& node)
{
	value.x = (float)node.at("x").ToFloat();
	value.y = (float)node.at("y").ToFloat();
}

template <>
inline void load_from_json(SDL_Rect& value, const json::JSON& node)
{
	value.x = node.at("x").ToInt();
	value.y = node.at("y").ToInt();
	value.w = node.at("width").ToInt();
	value.h = node.at("height").ToInt();
}

template <>
inline void load_from_json(SDL_Color& value, const json::JSON& node)
{
	value.r = (unsigned char)node.at(0).ToInt();
	value.g = (unsigned char)node.at(1).ToInt();
	value.b = (unsigned char)node.at(2).ToInt();
	value.a = (unsigned char)node.at(3).ToInt();
}

#endif