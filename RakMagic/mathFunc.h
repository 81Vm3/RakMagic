#ifndef MATH_H
#define MATH_H

#ifndef M_PI
#define M_PI	3.14159265358979323844f
#endif
#define DEGTORAD( Deg ) ( (Deg * M_PI) / 180.0f )

int gen_gpci(char buf[64], unsigned long factor);

int		near_zero(float v);
void	vect2_normalize(const float in[2], float out[2]);
float	vect2_length(const float in[2]);
int		vect2_near_zero(const float in[2]);
void	vect2_copy(const uint8_t in[2], uint8_t out[2]);
void	vect3_zero(float out[3]);
void	vect3_normalize(const float in[3], float out[3]);
void	vect3_mult(const float in[3], float m, float out[3]);
float	vect3_length(const float in[3]);
void	vect3_div(const float in[3], float m, float out[3]);
void	vect3_mult(const float in[3], float m, float out[3]);
void	vect3_invert(const float in[3], float out[3]);
void	vect3_vect3_mult(const float in1[3], const float in2[3], float out[3]);
void	vect3_vect3_add(const float in1[3], const float in2[3], float out[3]);
void	vect3_vect3_sub(const float in1[3], const float in2[3], float out[3]);
int		vect3_near_zero(const float in[3]);
void	vect3_copy(const float in[3], float out[3]);
float	vect3_dist(const float in1[3], const float in2[3]);
int		vect4_near_zero(const float in[4]);
void	vect4_copy(const uint8_t in[4], uint8_t out[4]);
void	vect4_copy(const float in[4], float out[4]);
void	matrix_copy(const float in[16], float out[16]);
float	vect3_dot_product(const float in1[3], const float in2[3]);
void	vect3_cross_product(const float in1[3], const float in2[3], float out[3]);
void	matrix_vect3_mult(const float matrix[16], const float in[3], float out[3]);
void	matrix_vect4_mult(const float matrix[16], const float in[4], float out[4]);
void	matrix_matrix_mult(const float in1[16], const float in2[16], float out[16]);
void	matrix_vect3_rotate(const float in[16], const float vect[3], float t, float out[16]);
void	matrix_vect3_switchXY(const float in[16], float out[16]);
void	matrix_identity(float out[16]);

int		project(const float in[3], const float model[16], const float proj[16], float out[3]);

// BitStream::WriteOrthMatrix
void GetOrthMatrix(float m00, float m01, float m02,
	float m10, float m11, float m12,
	float m20, float m21, float m22,
	float *fOutW, float *fOutX, float *fOutY, float *fOutZ);
// BitStream::ReadOrthMatrix
void MakeOrthMatrix(float fInW, float fInX, float fInY, float fInZ,
	float *m00, float *m01, float *m02,
	float *m10, float *m11, float *m12,
	float *m20, float *m21, float *m22);

void BIG_NUM_MUL(unsigned long in[5], unsigned long out[6], unsigned long factor);

#endif