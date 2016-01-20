#pragma once
#ifndef PARAM_AFFICHAGE_DIM_HPP
#define PARAM_AFFICHAGE_DIM_HPP

#include "SFGUI\Frame.hpp"
#include "SFGUI\Scale.hpp"
#include "SFGUI\Box.hpp"
#include <map>

class ParamAffichageDim : public sfg::Frame
{
public:
	typedef std::shared_ptr<ParamAffichageDim> Ptr;

	ParamAffichageDim(int dim);
	~ParamAffichageDim();
	static std::shared_ptr<ParamAffichageDim> Create(int dim);
	const std::string& GetName() const override;

	float getRed();
	float getBlue();
	float getGreen();
	float getAlpha();
	int getDimension();
private:
	int dimension;
	std::map<std::string, sfg::Scale::Ptr> scalers;//curseurs mapés par leur nom
	sfg::Box::Ptr hBox;
	void initScales();
};

#endif