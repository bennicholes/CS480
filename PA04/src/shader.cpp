#include <fstream>
#include <string.h>
#include "shader.h"

using namespace std;


// intializes the array that will hold the shader attributes
ShaderLoader::ShaderLoader( const int bufferSize ){

    // variables


    shaderInfo = new char [ bufferSize ];
}


// destructor for shader loader
ShaderLoader::~ShaderLoader(){

    // variables

    delete[] shaderInfo;
}

/* reads and concatenates shader attributes the array is cleared each run*/
void ShaderLoader::loadShader( const char *fileName ){

    // variables
    char *currentInput = new char [ 1000 ];
    ifstream shader( fileName );
   
    int index;

    for( index = 0; index < 1000; index++ ){
    shaderInfo[ index ] = '\0';
    currentInput[ index ] = '\0';
    }


    while( shader.good() ){
        shader.getline( currentInput, 256 );
        strcat( shaderInfo, currentInput );
        shaderInfo[strlen(shaderInfo)] = '\n';
    }
}

// returns the pointer to the array holding the shader
const char* ShaderLoader::getShader(){

    // variables
    return shaderInfo;
}


