#include <fstream>
#include <string.h>
#include "shader.h"

using namespace std;

ShaderLoader::ShaderLoader( const int bufferSize ){

    // variables


    shaderInfo = new char [ bufferSize ];
}



ShaderLoader::~ShaderLoader(){

    // variables

    delete[] shaderInfo;
}


void ShaderLoader::loadShader( const char *fileName ){

    // variables
    char *currentInput = new char [ 10000 ];
    ifstream shader( fileName );
   



    while( shader.good() ){
        shader.getline( currentInput, 256 );
        strcat( shaderInfo, currentInput );
        shaderInfo[strlen(shaderInfo)] = '\n';
    }
}

const char* ShaderLoader::getShader(){

    // variables
    return shaderInfo;
}


