#pragma once
#include <fvec.h>
#include <cmath>
#include <iostream>
#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)
#define LERP(a,b,f) ((b - a)*f + a)

//Fixed Point Arithmetic and constants
#define PVAL 65536.0f
#define PSCAL 16
#define MULFP(a,b) ( (( (a) ) * ( (b) )) >>PSCAL )
#define DIVFP(a,b) ( (( (a) <<PSCAL) / ( (b) )) )

struct VEC4 {
	F32vec4 arr;
	VEC4() {
		arr[0] = 0;
		arr[1] = 0;
		arr[2] = 0;
		arr[3] = 0;
	}
	VEC4(const VEC4& otherVEC4) {
		arr = otherVEC4.arr;
	}
	VEC4(const F32vec4& otherSSEVEC) {
		arr = otherSSEVEC;
	}
	VEC4(const float& x, const float&y, const float& z, const float& w) {
		arr[0] = x;
		arr[1] = y;
		arr[2] = z;
		arr[3] = w;
	}
	VEC4(const float& num) {
		arr[0] = num;
		arr[1] = num;
		arr[2] = num;
		arr[3] = num;
	}
	VEC4 Lerp(const VEC4& dest, float Factor)
	{
		return VEC4(((dest.arr - this->arr) *((F32vec4)Factor))+(this->arr));
	}
	VEC4 Rotate(VEC4 axis, float angle) {//EULER ROTATION
		float sinAngle = (float)sin(-angle);
		float cosAngle = (float)cos(-angle);
		VEC4 c(arr);
				//rotation on local x	- rotation on local y - rotation on local z
		return c.Cross(axis*sinAngle)	 +		c * cosAngle	+ axis * (c.Dot(axis*(1 - cosAngle)));
	}
	VEC4 Normalize() const{
		VEC4 vec;
		float mag= this->Length();
		vec.arr = arr /(F32vec4)mag;
		return vec;
	}
	VEC4 Cross(const VEC4& V) const {
		float x1 = arr[0], x2 = V.arr[0];
		float y1 = arr[1], y2 = V.arr[1];
		float z1 = arr[2], z2 = V.arr[2];
		float x = y1 * z2 - y2 * z1;
		float y = z1 * x2 - z2 * x1;
		float z = x1 * y2 - x2 * y1;

		return VEC4(x, y, z, 0.0f);

	}
	float Dot(const VEC4& V)const {
		VEC4 ans = arr * V.arr;
		return ans.arr[0] + ans.arr[1] + ans.arr[2] + ans.arr[3];
	}
	float getMax() const{
		float max = arr[0];
		for (int16_t i = 1; i < 4; i++) {
			if (max < arr[i])max = arr[i];
		}
		return max;
	}
	float getMin() const {
		float min = arr[0];
		for (int16_t i = 1; i < 4; i++) {
			if (min > arr[i])min= arr[i];
		}
		return min;
	}
	VEC4 operator +(const VEC4& otherVEC4) const {
		VEC4 v(this->arr + otherVEC4.arr);
		return v;
	}
	VEC4 operator +(const float& f) const {
		VEC4 v(this->arr + (F32vec4)f);
		return v;
	}
	VEC4 operator -(const VEC4& otherVEC4) const {
		VEC4 v(this->arr - otherVEC4.arr);
		return v;
	}
	VEC4 operator -(const float& f) const {
		VEC4 v(this->arr - (F32vec4)f);
		return v;
	}
	VEC4 operator *(const VEC4& otherVEC4) const {
		VEC4 v(this->arr * otherVEC4.arr);
		return v;
	}
	VEC4 operator *(const float& f) const {
		VEC4 v(this->arr * (F32vec4)f);
		return v;
	}
	VEC4 operator /(const VEC4& otherVEC4) const {
		VEC4 v(this->arr / otherVEC4.arr);
		return v;
	}
	VEC4 operator /(const float& f) const {
		VEC4 v(this->arr / (F32vec4)f);
		return v;
	}
	float Length() const{
		F32vec4 SSE_VEC_SQUARED = arr * arr;
		return sqrtf(SSE_VEC_SQUARED[0] + SSE_VEC_SQUARED[1] + SSE_VEC_SQUARED[2] + SSE_VEC_SQUARED[3]);
	}
};
struct VEC3 {
	F32vec4 arr;
	VEC3() {
		arr[0] = 0;
		arr[1] = 0;
		arr[2] = 0;
		arr[3] = 0.0f;
	}
	VEC3(const VEC3& otherVEC3) {
		arr = otherVEC3.arr;
	}
	VEC3(const F32vec4& otherSSEVEC) {
		arr = otherSSEVEC;
	}
	VEC3(const float& x, const float&y, const float& z) {
		arr[0] = x;
		arr[1] = y;
		arr[2] = z;
		arr[3] = 0.0f;
	}
	VEC3(const float& num) {
		arr[0] = num;
		arr[1] = num;
		arr[2] = num;
		arr[3] = 0.0f;
	}
	VEC3 Normalize() const{
		VEC3 vec;
		float mag = this->Length();
		vec.arr = arr / (F32vec4)mag;
		return vec;
	}
	VEC3 Cross(const VEC3& V) const {
		float x1 = arr[0], x2 = V.arr[0];
		float y1 = arr[1], y2 = V.arr[1];
		float z1 = arr[2], z2 = V.arr[2];
		float x = y1 * z2 - y2 * z1;
		float y = z1 * x2 - z2 * x1;
		float z = x1 * y2 - x2 * y1;

		return VEC3(x, y, z);

	}
	float Dot(const VEC3& V)const {
		VEC3 ans = arr * V.arr;//EXPLICIT CASTING
		return ans.arr[0] + ans.arr[1] + ans.arr[2];
	}
	float getMax() const {
		float max = arr[0];
		for (int16_t i = 1; i < 3; i++) {
			if (max < arr[i])max = arr[i];
		}
		return max;
	}
	float getMin() const {
		float min = arr[0];
		for (int16_t i = 1; i < 3; i++) {
			if (min > arr[i])min = arr[i];
		}
		return min;
	}
	VEC3 operator +(const VEC3& otherVEC3) const {
		VEC3 v(this->arr + otherVEC3.arr);
		return v;
	}
	VEC3 operator +(const float& f) const {
		VEC3 v(this->arr + (F32vec4)f);
		return v;
	}
	VEC3 operator -(const VEC3& otherVEC3) const {
		VEC3 v(this->arr - otherVEC3.arr);
		return v;
	}
	VEC3 operator -(const float& f) const {
		VEC3 v(this->arr - (F32vec4)f);
		return v;
	}
	VEC3 operator *(const VEC3& otherVEC3) const {
		VEC3 v(this->arr * otherVEC3.arr);
		return v;
	}
	VEC3 operator *(const float& f) const {
		VEC3 v(this->arr * (F32vec4)f);
		return v;
	}
	VEC3 operator /(const VEC3& otherVEC3) const {
		VEC3 v(this->arr / otherVEC3.arr);
		return v;
	}
	VEC3 operator /(const float& f) const {
		VEC3 v(this->arr / (F32vec4)f);
		return v;
	}
	float Length() const {
		F32vec4 SSE_VEC_SQUARED = arr * arr;
		return sqrtf(SSE_VEC_SQUARED[0] + SSE_VEC_SQUARED[1] + SSE_VEC_SQUARED[2]);
	}
};
struct VEC2 {
	F32vec4 arr;
	VEC2() {
		arr[0] = 0.0f;
		arr[1] = 0.0f;
		arr[2] = 0.0f;
		arr[3] = 0.0f;
	}
	VEC2(const VEC2& otherVEC2) {
		arr = otherVEC2.arr;
	}
	VEC2(const F32vec4& otherSSEVEC) {
		arr = otherSSEVEC;
	}
	VEC2(const float& x, const float& y) {
		arr[0] = x;
		arr[1] = y;
		arr[2] = 0.0f;
		arr[3] = 0.0f;
	}
	VEC2(const float& num) {
		arr[0] = num;
		arr[1] = num;
		arr[2] = 0.0f;
		arr[3] = 0.0f;
	}
	float Dot(const VEC2& V)const {
		VEC2 ans = arr * V.arr;//EXPLICIT CASTING
		return ans.arr[0] + ans.arr[1];
	}
	float getMax() const {
		float max = arr[0];
			if (max < arr[1])max = arr[1];
		return max;
	}
	float getMin() const {
		float min = arr[0];
			if (min > arr[1])min = arr[1];
		return min;
	}
	VEC2 operator +(const VEC2& otherVEC2) const {
		VEC2 v(this->arr + otherVEC2.arr);
		return v;
	}
	VEC2 operator +(const float& f) const {
		VEC2 v(this->arr + (F32vec4)f);
		return v;
	}
	VEC2 operator -(const VEC2& otherVEC2) const {
		VEC2 v(this->arr - otherVEC2.arr);
		return v;
	}
	VEC2 operator -(const float& f) const {
		VEC2 v(this->arr - (F32vec4)f);
		return v;
	}
	VEC2 operator *(const VEC2& otherVEC2) const {
		VEC2 v(this->arr * otherVEC2.arr);
		return v;
	}
	VEC2 operator *(const float& f) const {
		VEC2 v(this->arr * (F32vec4)f);
		return v;
	}
	VEC2 operator /(const VEC2& otherVEC2) const {
		VEC2 v(this->arr / otherVEC2.arr);
		return v;
	}
	VEC2 operator /(const float& f) const {
		VEC2 v(this->arr / (F32vec4)f);
		return v;
	}
	float Length() const {
		F32vec4 SSE_VEC_SQUARED = arr * arr;
		return sqrtf(SSE_VEC_SQUARED[0] + SSE_VEC_SQUARED[1]);
	}
};
struct Color {

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	Color() {
		r = 0xff;
		g = 0xff;
		b = 0xff;
		a = 0xff;
	}
	Color(const uint32_t& colorValue) {
		this->r= colorValue>>24;
		this->g = colorValue >> 16;
		this->b = colorValue >> 8;
		this->a = colorValue;

	}
	Color(const Color& otherColor) {
		this->r = otherColor.r;
		this->g = otherColor.g;
		this->b = otherColor.b;
		this->a = otherColor.a;
	}
	Color(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	inline uint32_t get32BitValue() const {
		return static_cast<uint32_t>(this->r << 24 | this->g << 16 | this->b << 8 | this->a << 0);
	}
	Color operator +(const Color& otherColor) const{
		Color v(this->r + otherColor.r, this->g + otherColor.g, this->b + otherColor.b, this->a + otherColor.a);
		return v;
	}
	Color operator -(const Color& otherColor) const{
		Color v(this->r - otherColor.r, this->g - otherColor.g, this->b - otherColor.b, this->a - otherColor.a);
		return v;
	}
	Color operator *(const Color& otherColor) const{
		Color v(this->r * otherColor.r, this->g * otherColor.g, this->b * otherColor.b, this->a * otherColor.a);
		return v;
	}
	Color operator /(const Color& otherColor) const{
		Color v(this->r / otherColor.r, this->g / otherColor.g, this->b / otherColor.b, this->a / otherColor.a);
		return v;
	}


};
struct MAT4 {
	VEC4 rows[4];
	MAT4() {//make identity matrix
		for (int16_t i = 0; i < 4; i++) {
			rows[i].arr[i] = 1;
		}
	}
	MAT4(const MAT4& otherMAT4) {
		for (int16_t i = 0; i < 4; i++) {
			rows[i] = otherMAT4.rows[i];
		}
	}
	MAT4(const float& num) {
		for (int16_t i = 0; i < 4; i++) {
			rows[i].arr[0] = num;
			rows[i].arr[1] = num;
			rows[i].arr[2] = num;
			rows[i].arr[3] = num;
		}
	}
	static MAT4 getScrSpaceTransform(const float& halfWidth, const float& halfHeight){
		MAT4 m;
		m.rows[0].arr[0] = halfWidth;
		m.rows[1].arr[1] = -halfHeight;
		m.rows[0].arr[3] = halfWidth;
		m.rows[1].arr[3] = halfHeight;
		return m;
	}
	static MAT4 getTranslation(const float& x, const float& y, const float&z){
		MAT4 m;
		m.rows[0].arr[3] = x;
		m.rows[1].arr[3] = y;
		m.rows[2].arr[3] = z;
		return m;
	}
	static MAT4 getRotation(const float& x, const float& y, const float& z, const float& angle){
		float s = (float)sin(angle);
		float c = (float)cos(angle);
		float Ic = 1.0f - c;
		MAT4 m;
		m.rows[0].arr[0] = c + x * x*Ic; m.rows[0].arr[1] = x * y*Ic - z * s; m.rows[0].arr[2] = x * z*Ic + y * s;
		m.rows[1].arr[0] = y * x*Ic + z * s; m.rows[1].arr[1] = c + y * y*Ic; m.rows[1].arr[2] = y * z*Ic - x * s;
		m.rows[2].arr[0] = z * x*Ic - y * s; m.rows[1].arr[1] = z * y*Ic + x * s; m.rows[1].arr[2] = c + z * z*Ic;
		return m;

	}
	static MAT4 getRotation(const float& x, const float& y, const float& z){
		MAT4 RX;
		MAT4 RY;
		MAT4 RZ;
		RZ.rows[0].arr[0] = cos(z); RZ.rows[0].arr[1] = -sin(z);
		RZ.rows[1].arr[0] = sin(z); RZ.rows[1].arr[1] = cos(z);		

		RY.rows[0].arr[0] = cos(y); RY.rows[0].arr[2] = -sin(y);
		RY.rows[2].arr[0] = sin(y); RY.rows[2].arr[2] = cos(y);

		RX.rows[1].arr[1] = cos(x); RX.rows[1].arr[2] = -sin(x);
		RX.rows[2].arr[1] = sin(x); RX.rows[2].arr[2] = cos(x);
	
		
		return  RZ * RY * RX;

	}
	static MAT4 getScale(const float& x, const float& y, const float& z)
	{
		MAT4 m;
		m.rows[0].arr[0] = x;m.rows[1].arr[1] = y;	m.rows[2].arr[2] = z;
		return m;
	}

	static MAT4 getPerspective(const float& FOV,const float& aspectRatio,const float& zNear,const float& zFar)
	{
		float tanHalfFOV =tanf((FOV / 2.0f)*(3.14159265359f/180.0f));
		float zRange = zNear - zFar;
		MAT4 m;
		m.rows[0].arr[0] = 1.0f / (tanHalfFOV * aspectRatio);
		m.rows[1].arr[1] = 1.0f / tanHalfFOV;
		m.rows[2].arr[2] = (-zNear - zFar) / zRange;
		m.rows[2].arr[3] = 2 * zFar * zNear / zRange;
		m.rows[3].arr[2] = 1.0f;
		m.rows[3].arr[3] = 0.0f;
		
		return m;
	}

	static MAT4 getOrthographic(const float& left, const float& right, const float& bottom, const float& top, const float& near, const float& far)
	{
		float width = right - left;
		float height = top - bottom;
		float depth = far - near;
		MAT4 m;
		m.rows[0].arr[0] = 2 / width;	
		m.rows[0].arr[3] = -(right + left) / width;
		m.rows[1].arr[1] = 2 / height;
		m.rows[1].arr[3] = -(top + bottom) / height;
		m.rows[2].arr[2] = -2 / depth;
		m.rows[2].arr[3] = -(far + near) / depth;

		return m;
	}

	static MAT4 getRotation(const VEC4& forward,const VEC4& up)
	{
		VEC4 f = forward.Normalize();

		VEC4 r = up.Normalize();
		r = r.Cross(f);

		VEC4 u = f.Cross(r);

		return MAT4::getRotation(f, u, r);
	}

	static MAT4 getRotation(const VEC4& forward,const VEC4& up,const VEC4& right)
	{
		VEC4 f = forward;
		VEC4 r = right;
		VEC4 u = up;
		MAT4 m;
		m.rows[0].arr[0] = r.arr[0];	m.rows[0].arr[1] = r.arr[1];	m.rows[0].arr[2] = r.arr[2];
		m.rows[1].arr[0] = u.arr[0];	m.rows[1].arr[1] = u.arr[1];	m.rows[1].arr[2] = u.arr[2];	
		m.rows[2].arr[0] = f.arr[0];	m.rows[2].arr[1] = f.arr[1];	m.rows[2].arr[2] = f.arr[2];			

		return m;
	}

	VEC4 Transform(const VEC4& r) const
	{
		return VEC4(rows[0].Dot(r),rows[1].Dot(r),rows[2].Dot(r),rows[3].Dot(r));
	}

	MAT4 operator *(const MAT4& otherMAT4) const {
		MAT4 m;
		VEC4 tmp;
		for (int16_t i = 0; i < 4; i++) {//iterate through other mat cols
			tmp.arr[0] = otherMAT4.rows[0].arr[i];
			tmp.arr[1] = otherMAT4.rows[1].arr[i];
			tmp.arr[2] = otherMAT4.rows[2].arr[i];
			tmp.arr[3] = otherMAT4.rows[3].arr[i];
			m.rows[0].arr[i] = rows[0].Dot(tmp);
			m.rows[1].arr[i] = rows[1].Dot(tmp);
			m.rows[2].arr[i] = rows[2].Dot(tmp);
			m.rows[3].arr[i] = rows[3].Dot(tmp);
		}
		return m;
	}

};
struct Vertex {
	VEC4 pos;
	VEC2 texCoords;
	VEC3 normal;
	Color col;
	Vertex() {
		this->pos = VEC4();
		pos.arr[3] = 1.0f;
		this->texCoords = VEC2();
		this->normal = VEC3();
		this->col = Color();
	}
	Vertex(const float& x,const float& y,const float& z
		,const float& u, const float& v
		,const float& nx,const float& ny,const float &nz
		,const uint32_t& col) {
		this->pos = VEC4(x, y,z,1.0f);
		this->texCoords = VEC2(u,v);
		this->normal = VEC3(nx, ny, nz);
		this->col = Color(col);
	}
	Vertex(const VEC4& pos,const VEC2& texCoords,const VEC3& normal,const Color& col) {
		this->pos = VEC4(pos);
		this->texCoords = texCoords;
		this->normal = normal;
		this->col = Color(col);
	}
	Vertex(const VEC4& pos, const VEC2& texCoords, const VEC3& normal) {
		this->pos = VEC4(pos);
		this->texCoords = texCoords;
		this->normal = normal;
		this->col = Color();
	}
	Vertex Transform(const MAT4& transMat) const
	{
		return Vertex(transMat.Transform(pos), this->texCoords, this->normal, this->col);
	}

	Vertex PrespDivide() const
	{
		return Vertex(VEC4(pos.arr[0] / pos.arr[3], pos.arr[1] / pos.arr[3],
			pos.arr[2] / pos.arr[3], pos.arr[3]), this->texCoords,this->normal,this->col);
	}

};