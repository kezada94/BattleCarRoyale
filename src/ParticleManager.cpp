#include "ParticleManager.hpp"
#include "stb_image.h"
ParticleManager::ParticleManager(){
    this->shader_programme = create_programme_from_files ("res/shaders/particles.vert", "res/shaders/particles.frag");
    this->time_location = glGetUniformLocation(this->shader_programme, "elapsed_system_time");
    this->view_location = glGetUniformLocation(this->shader_programme, "V");
    this->proj_location = glGetUniformLocation(this->shader_programme, "P");

    load_texture( "res/Droplet.png", &tex );

    /*for (int i = 0; i < MAX_PARTICLES; i++){
        genBullet();
    }*/
}

void ParticleManager::genGunshot(btVector3 from, btVector3 to){
    float velocityDir[3]; 
    float origin[3]={from.x(),from.y(),from.z()};
	double vt = currentTime;
    btVector3 dir = to-from;
    dir = dir.normalized();
    velocityDir[0] = dir.x();		
    velocityDir[1] = dir.y();
    velocityDir[2] = dir.z();
	
    GLuint velocity_vbo;
    glGenBuffers (1, &velocity_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, velocity_vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat)*3, &velocityDir, GL_STATIC_DRAW);
    
    GLuint origin_vbo;
    glGenBuffers (1, &origin_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, origin_vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat)*3, &origin, GL_STATIC_DRAW);
    
    GLuint vao;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glBindBuffer (GL_ARRAY_BUFFER, velocity_vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer (GL_ARRAY_BUFFER, origin_vbo);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    float distanc = (to-from).length();

    float t = distanc/420.f;

    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);
    
    ParticleSystem* par = new ParticleSystem;
    par->startTime = vt;
    par->endTime = vt + t; //TODO: PARAM 1.3 segundos de vida
    par->vao = vao; 
    this->activeSystems.push_back(par);        
}
void ParticleManager::drawActiveParticles(){    
    
    for(int i = 0; i < this->activeSystems.size(); i++){
        
        ParticleSystem* p = this->activeSystems.at(i);
        
        if (currentTime < p->endTime){
            glEnable (GL_PROGRAM_POINT_SIZE);
            glActiveTexture( GL_TEXTURE0 );
            glBindTexture( GL_TEXTURE_2D, tex );
            glUseProgram (this->shader_programme);
            glUniform1f (time_location, (currentTime - p->startTime));         
            glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(*view));
            glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(*proj));
            
            glBindVertexArray (p->vao);
            // draw points 0-3 from the currently bound VAO with current in-use shader
            glDrawArrays (GL_POINTS, 0, 1);
            glDisable (GL_PROGRAM_POINT_SIZE);
            
        }else{
            delete p;
            activeSystems.erase(activeSystems.begin()+i);
            glDeleteVertexArrays(1, &(p->vao));
            //deletebuffer
        }
    }
}
void ParticleManager::setView(glm::mat4* vi){
    this->view  = vi;
}
void ParticleManager::setProj(glm::mat4* pro){
    this->proj = pro;
}

bool ParticleManager::load_texture( const char *file_name, GLuint *tex ) {
	int x, y, n;
	int force_channels = 4;
	unsigned char *image_data = stbi_load( file_name, &x, &y, &n, force_channels );
	if ( !image_data ) {
		fprintf( stderr, "ERROR: could not load %s\n", file_name );
		return false;
	}
	// NPOT check
	if ( ( x & ( x - 1 ) ) != 0 || ( y & ( y - 1 ) ) != 0 ) {
		fprintf( stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name );
	}
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for ( int row = 0; row < half_height; row++ ) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + ( y - row - 1 ) * width_in_bytes;
		for ( int col = 0; col < width_in_bytes; col++ ) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
	glGenTextures( 1, tex );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, *tex );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data );
	glGenerateMipmap( GL_TEXTURE_2D );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	GLfloat max_aniso = 0.0f;
	glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso );
	// set the maximum!
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso );
	return true;
}