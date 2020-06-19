#ifndef _MATERIAL_SHADERHELPER_H_
#define _MATERIAL_SHADERHELPER_H_

#include <string>

class VertexShader;
class FragmentShader;

class ShaderHelper
{
public:
    static VertexShader* LoadBaseVertexShader();
    static FragmentShader* LoadBaseNoLitFragmentShader();

    static VertexShader* LoadBaseCurveVertexShader();
    static FragmentShader* LoadBaseCurveFragmentShader();

	static VertexShader* LoadEngineNormalVertexShader();
	static FragmentShader* LoadEngineNormalFragmentShader();

    static VertexShader* LoadVertexShader(const std::string& shaderName);
    static FragmentShader* LoadFragmentShader(const std::string& shaderName);
};

#endif
