#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/freeglut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <chrono>
#include <vector>
#define GLM_FORCE_RADIANS // force radians for rotate function
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "shader.h" // file for shader loader

//--Data types
//This object will define the attributes of a vertex(position, color, etc...)
struct Vertex
{
    GLfloat position[3];
    GLfloat color[3];
};

//--Evil Global variables
//Just for this example!
int w = 640, h = 480;// Window size
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry
int numVertices;

// flag for exit
bool exitFlag = false;

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint loc_color;

//transform matrices
glm::mat4 model;//obj->world each object should have its own planet matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);
void keyboard(unsigned char key, int x_pos, int y_pos);
void mouse( int button, int state, int x_pos, int y_pos );
void menu( int menuChoice );

//--Resource management
bool initialize(char * argv);
void cleanUp();

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

Vertex* loadObject( const char *file );

//--Main
int main(int argc, char **argv)
{

    if( argc != 2 ){
        std::cout<<"This program needs the file.";
        exit(1);
    }
    // Initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);
    // Name and create the Window
    glutCreateWindow("Matrix Example");
    
    // create menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Exit", 0);  
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Now that the window is created the GL context is fully set up
    // Because of that we can now initialize GLEW to prepare work with shaders
    GLenum status = glewInit();
    if( status != GLEW_OK)
    {
        std::cerr << "[F] GLEW NOT INITIALIZED: ";
        std::cerr << glewGetErrorString(status) << std::endl;
        return -1;
    }

    // Set all of the callbacks to GLUT that we need
    glutDisplayFunc(render);// Called when its time to display
    glutReshapeFunc(reshape);// Called if the window is resized
    glutIdleFunc(update);// Called if there is nothing else to do
    glutKeyboardFunc(keyboard);// Called if there is keyboard input
    glutMouseFunc(mouse); // Called if there is mouse input

    // Initialize all of our resources(shaders, geometry)
    bool init = initialize(argv[1]);
    if(init)
    {
        t1 = std::chrono::high_resolution_clock::now();
        glutMainLoop();
    }

    // Clean up after ourselves
    cleanUp();
    return 0;
}

//--Implementations
void render()
{
    //--Render the scene

    //clear the screen
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //premultiply the matrix for this example
    mvp = projection * view * model;

    //enable the shader program
    glUseProgram(program);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    //set pointers into the vbo for each of the attributes(position and color)
    glVertexAttribPointer( loc_position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset

    glVertexAttribPointer( loc_color,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(Vertex),
                           (void*)offsetof(Vertex,color));

    glDrawArrays(GL_TRIANGLES, 0, numVertices);//mode, starting index, count

    //clean up
    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_color);
      
    //swap the buffer
    glutSwapBuffers();
}

void update()
{
    // check if program should exitglClear
    if(exitFlag){
      // glutLeaveMainLoop();
        exit(0);
    }
    // Update the state of the scene
    glutPostRedisplay();//call the display callback
}


void reshape(int n_w, int n_h)
{
    w = n_w;
    h = n_h;
    //Change the viewport to be correct
    glViewport( 0, 0, w, h);
    //Update the projection matrix as well
    //See the init function for an explaination
    projection = glm::perspective(45.0f, float(w)/float(h), 0.01f, 100.0f);

}


// Takes keyboard input
void keyboard(unsigned char key, int x_pos, int y_pos)
{
    // Handle keyboard input
    if(key == 27)//ESC
    {
        exitFlag = true;
    }

}


// takes mouse input
void mouse( int button, int state, int x_pos, int y_pos){

    // variables

    // take mouse input
    // change rotation clockwise or counterclockwise
    // if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ){
    
    //     rotationDirection *= -1; 
    // }
}

void menu( int menuChoice ){

    // variables

    if( menuChoice == 0  )
        exitFlag = true;
}


bool initialize( char* argv)
{
    // variables
    ShaderLoader vertexShader, fragmentShader;
    
    // Initialize basic geometry and shaders for this example

    // load object
    Vertex *geometry = loadObject(argv);
                        
    // Create a Vertex Buffer object to store this vertex info on the GPU
    glGenBuffers(1, &vbo_geometry);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), geometry, GL_STATIC_DRAW);

    //--Geometry done

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    //Shader Sources
    // Put these into files and write a loader in the future
    // Note the added uniform!

    // load shader and fragment files and pass to gl compiler
    vertexShader.loadShader("../bin/matrix.vert");
    const char *vs = vertexShader.getShader();

    fragmentShader.loadShader("../bin/matrix.frag");
    const char *fs = fragmentShader.getShader();

    //compile the shaders
    GLint shader_status;

    // Vertex shader first
    glShaderSource(vertex_shader, 1, &vs, NULL);
    glCompileShader(vertex_shader);
    //check the compile status
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE VERTEX SHADER!" << std::endl;
        return false;
    }

    // Now the Fragment shader
    glShaderSource(fragment_shader, 1, &fs, NULL);
    glCompileShader(fragment_shader);
    //check the compile status
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE FRAGMENT SHADER!" << std::endl;
        return false;
    }

    //Now we link the 2 shader objects into a program
    //This program is what is run on the GPU
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    //check if everything linked ok
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
        return false;
    }

    //Now we set the locations of the attributes and uniforms
    //this allows us to access them easily while rendering
    loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
        return false;
    }

    loc_color = glGetAttribLocation(program,
                    const_cast<const char*>("v_color"));
    if(loc_color == -1)
    {
        std::cerr << "[F] V_COLOR NOT FOUND" << std::endl;
        return false;
    }

    loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
        return false;
    }
    
    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    view = glm::lookAt( glm::vec3(0.0, 8.0, -16.0), //Eye Position
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   100.0f); //Distance to the far plane, 

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //and its done
    return true;
}

void cleanUp()
{
    // Clean up, Clean up
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry);
}

//returns the time delta
float getDT()
{
    float ret;
    t2 = std::chrono::high_resolution_clock::now();
    ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
    t1 = std::chrono::high_resolution_clock::now();
    return ret;
}

// object loader
Vertex* loadObject( const char *filePath ){
  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector< glm::vec3 > temp_vertices;
  std::vector< glm::vec2 > temp_uvs;
  std::vector< glm::vec3 > temp_normals;
  bool hasTextures = false;
  bool hasNormals = false;

  FILE *file = fopen(filePath, "r");
  if( file == NULL ){
    printf("Can't open file \n");
    // return false;
  }

  while( 1 ){
    
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file , "%s", lineHeader);
    if (res == EOF)
      break; // EOF = End Of File. Quit the loop.

    // else : parse lineHeader
    if ( strcmp( lineHeader, "v" ) == 0 )
    {
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
      temp_vertices.push_back(vertex);
    }
    else if ( strcmp( lineHeader, "vt" ) == 0 )
    {
      hasTextures = true;
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y );
      temp_uvs.push_back(uv);
    }
    else if ( strcmp( lineHeader, "vn" ) == 0 )
    {
      hasNormals = true;
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
      temp_normals.push_back(normal);
    }
    else if ( strcmp( lineHeader, "f" ) == 0 )
    {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

      if(!hasTextures && !hasNormals)
      {
      int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2] );
        if (matches != 3)
        {
            printf("File can't be read by our simple parser : ( Try exporting with other options\n");
            //return false;
        }
        vertexIndices.push_back(vertexIndex[0]);
        vertexIndices.push_back(vertexIndex[1]);
        vertexIndices.push_back(vertexIndex[2]);
      }

      else if(hasTextures && !hasNormals)
      {
        int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2] );
        if (matches != 6)
        {
            printf("File can't be read by our simple parser : ( Try exporting with other options\n");
            //return false;
        }
        vertexIndices.push_back(vertexIndex[0]);
        vertexIndices.push_back(vertexIndex[1]);
        vertexIndices.push_back(vertexIndex[2]);
      }

      else if(!hasTextures && hasNormals)
      {
        int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0],  &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] );
        if (matches != 6)
        {
            printf("File can't be read by our simple parser : ( Try exporting with other options\n");
            //return false;
        }
        vertexIndices.push_back(vertexIndex[0]);
        vertexIndices.push_back(vertexIndex[1]);
        vertexIndices.push_back(vertexIndex[2]);
      }

      else if(hasTextures && hasNormals)
      {
        int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
        if (matches != 9)
        {
            printf("File can't be read by our simple parser : ( Try exporting with other options\n");
            //return false;
        }
        vertexIndices.push_back(vertexIndex[0]);
        vertexIndices.push_back(vertexIndex[1]);
        vertexIndices.push_back(vertexIndex[2]);
      }
    }
  }
  // For each vertex of each triangle
  Vertex * geometry = new Vertex[ vertexIndices.size() ];
  numVertices = vertexIndices.size();
  for( unsigned int i = 0; i < vertexIndices.size(); i++ )
  {
    unsigned int vertexIndex = vertexIndices[i];
    geometry[i].position[0] = temp_vertices[ vertexIndex-1 ].x;
    geometry[i].position[1] = temp_vertices[ vertexIndex-1 ].y;
    geometry[i].position[2] = temp_vertices[ vertexIndex-1 ].z;

    geometry[i].color[0] = 1.0f / ( (i % 5) + 1.0f) ;

  }
  return geometry;
}








































