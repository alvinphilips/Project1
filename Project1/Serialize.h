#pragma once

#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "json.hpp"

template <typename T>
void load_from_json(T& value, const json::JSON& node);

template <typename T>
json::JSON save_to_json(const T& value);

template <>
inline void load_from_json(int& value, const json::JSON& node)
{
	value = node.ToInt();
}

template <>
inline json::JSON save_to_json(const int& value) {
	json::JSON node = value;

	return node;
}

template <>
inline void load_from_json(unsigned int& value, const json::JSON& node)
{
	value = (unsigned int)node.ToInt();
}

template <>
inline json::JSON save_to_json(const unsigned int& value) {
	json::JSON node = value;

	return node;
}

template <>
inline void load_from_json(bool& value, const json::JSON& node)
{
	value = node.ToBool();
}

template <>
inline json::JSON save_to_json(const bool& value) {
	json::JSON node = value;

	return node;
}

template <>
inline void load_from_json(std::string& value, const json::JSON& node)
{
	value = node.ToString();
}

template <>
inline json::JSON save_to_json(const std::string& value) {
	json::JSON node = value;

	return node;
}

template <>
inline void load_from_json(float& value, const json::JSON& node)
{
	value = (float)node.ToFloat();
}

template <>
inline json::JSON save_to_json(const float& value) {
	json::JSON node = value;

	return node;
}

template <>
inline void load_from_json(SDL_FPoint& value, const json::JSON& node)
{
	value.x = (float)node.at("x").ToFloat();
	value.y = (float)node.at("y").ToFloat();
}

template <>
inline json::JSON save_to_json(const SDL_FPoint& value) {
	json::JSON node;
	node["x"] = value.x;
	node["y"] = value.y;
	return node;
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
inline json::JSON save_to_json(const SDL_Rect& value) {
	json::JSON node;
	node["x"] = value.x;
	node["y"] = value.y;
	node["width"] = value.w;
	node["height"] = value.h;
	return node;
}

template <>
inline void load_from_json(SDL_Color& value, const json::JSON& node)
{
	value.r = (unsigned char)node.at(0).ToInt();
	value.g = (unsigned char)node.at(1).ToInt();
	value.b = (unsigned char)node.at(2).ToInt();
	value.a = (unsigned char)node.at(3).ToInt();
}

template <>
inline json::JSON save_to_json(const SDL_Color& value) {
	json::JSON node = json::JSON::Array();
	node.append(value.r);
	node.append(value.g);
	node.append(value.b);
	node.append(value.a);
	return node;
}

#endif