#pragma once

namespace ColacX
{
	class TriangleCompiler
	{
	public:
		static void convert( const char *fileIn, const char *fileOut );
		static int compile( const char* binaryFile );
	};
};

