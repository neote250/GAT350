#pragma once
#include "Transform.h"
#include "Model.h"
#include "Color.h"
#include <memory>


class Actor
{
public:
	Actor(const Transform& transform, std::shared_ptr<Model> model) : 
		_transform{ transform }
		, _model{ model }
	{}

	Transform& GetTransform() { return _transform; }
	const Transform& GetTransform() const { return _transform; }

	void Draw(class Framebuffer& framebuffer, const class Camera& camera);
	void SetColor(const color_t& color) { _color = color; }

private:
	Transform _transform;
	std::shared_ptr<Model> _model;
	color_t _color{ 255,255,255,255 };
};