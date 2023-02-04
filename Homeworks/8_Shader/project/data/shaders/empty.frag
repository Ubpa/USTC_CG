#version 330 core

void main()
{
	// empty
	//如果是正交投影矩阵，则深度值是线性的
	//// gl_FragDepth = gl_FragCoord.z;

	//如果使用透视投影矩阵，则深度值是非线性的，
	//如果要保存深度值，需要使用使其线性化
}