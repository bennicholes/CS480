/* class to laod shader attributes in main.cpp */

class ShaderLoader{

    public:
        ShaderLoader( const int bufferSize = 10000 );
        ~ShaderLoader();
        void loadShader( const char *fileName );
        const char* getShader();

    private:
        char *shaderInfo;
};
