#pragma once
#include <fvec.h>
#include <cmath>
#include <iostream>
//UNCOMMENT TO STOP USING fvec.h
/*
struct F32vec4 {
	float arr[4];
	F32vec4() {
		arr[0] = 0;
		arr[1] = 0;
		arr[2] = 0;
		arr[3] = 0;
	}
	F32vec4(float f1,float f2,float f3,float f4) {
		arr[0] = f1;
		arr[1] = f2;
		arr[2] = f3;
		arr[3] = f4;
	}
	F32vec4(float n) {
		arr[0] = n;
		arr[1] = n;
		arr[2] = n;
		arr[3] = n;
	}
	F32vec4 operator +(const F32vec4& other) const{
		F32vec4 v;
		for(int i=0;i<4;i++)
		v.arr[i] =this->arr[i] + other.arr[i];

		return v;
	}
	F32vec4 operator *(const F32vec4& other) const {
		F32vec4 v;
		for (int i = 0; i<4; i++)
			v.arr[i] = this->arr[i] * other.arr[i];

		return v;
	}
	F32vec4 operator -(const F32vec4& other) const{
		F32vec4 v;
		for (int i = 0; i<4; i++)
			v.arr[i] = this->arr[i] - other.arr[i];

		return v;
	}
	F32vec4 operator /(const F32vec4& other) const {
		F32vec4 v;
		for (int i = 0; i<4; i++)
			v.arr[i] = this->arr[i] / other.arr[i];

		return v;
	}
	F32vec4 operator +(const float& other) const {
		F32vec4 v;
		for (int i = 0; i < 4; i++)
			v.arr[i] = this->arr[i] + other;

		return v;
	}
	F32vec4 operator *(const float& other) const{
		F32vec4 v;
		for (int i = 0; i<4; i++)
			v.arr[i] = this->arr[i] * other;

		return v;
	}
	F32vec4 operator -(const float& other) const {
		F32vec4 v;
		for (int i = 0; i<4; i++)
			v.arr[i] = this->arr[i] - other;

		return v;
	}
	F32vec4 operator /(const float& other) const {
		F32vec4 v;
		for (int i = 0; i<4; i++)
			v.arr[i] = this->arr[i] / other;

		return v;
	}
	float operator [](const int& n) const {
		return arr[n];
	}
	float& operator [](const int& n){
		return arr[n];
	}
	
};*/
struct VEC4 {
	F32vec4 SSE_VEC;//using simd for better performance
	VEC4() {
		SSE_VEC[0] = 0;
		SSE_VEC[1] = 0;
		SSE_VEC[2] = 0;
		SSE_VEC[3] = 0;
	}
	VEC4(const VEC4& otherVEC4) {
		SSE_VEC = otherVEC4.SSE_VEC;
	}
	VEC4(const F32vec4& otherSSEVEC) {
		SSE_VEC = otherSSEVEC;
	}
	VEC4(const float& x, const float&y, const float& z, const float& w) {
		SSE_VEC[0] = x;
		SSE_VEC[1] = y;
		SSE_VEC[2] = z;
		SSE_VEC[3] = w;
	}
	VEC4(const float& num) {
		SSE_VEC[0] = num;
		SSE_VEC[1] = num;
		SSE_VEC[2] = num;
		SSE_VEC[3] = num;
	}
	VEC4 Lerp(const VEC4& dest, float Factor)
	{
		return VEC4(((dest.SSE_VEC - this->SSE_VEC) *((F32vec4)Factor))+(this->SSE_VEC));
	}
	VEC4 Rotate(VEC4 axis, float angle) {//EULER ROTATION
		float sinAngle = (float)sinf(-angle);
		float cosAngle = (float)cosf(-angle);
		VEC4 c(SSE_VEC);
				//rotation on local x	- rotation on local y - rotation on local z
		return c.Cross(axis*sinAngle)	 +		c * cosAngle	+ axis * (c.Dot(axis*(1 - cosAngle)));
	}
	VEC4 Normalize() const{
		VEC4 vec;
		float mag= this->Length();
		vec.SSE_VEC = SSE_VEC /(F32vec4)mag;
		return vec;
	}
	VEC4 Cross(const VEC4& V) const {
		float x1 = SSE_VEC[0], x2 = V.SSE_VEC[0];
		float y1 = SSE_VEC[1], y2 = V.SSE_VEC[1];
		float z1 = SSE_VEC[2], z2 = V.SSE_VEC[2];
		float x = y1 * z2 - y2 * z1;
		float y = z1 * x2 - z2 * x1;
		float z = x1 * y2 - x2 * y1;

		return VEC4(x, y, z, 0.0f);

	}
	float Dot(const VEC4& V)const {
		VEC4 ans = SSE_VEC * V.SSE_VEC;//EXPLICIT CASTING
		return ans.SSE_VEC[0] + ans.SSE_VEC[1] + ans.SSE_VEC[2] + ans.SSE_VEC[3];
	}
	float getMax() const{
		float max = SSE_VEC[0];
		for (int16_t i = 1; i < 4; i++) {
			if (max < SSE_VEC[i])max = SSE_VEC[i];
		}
		return max;
	}
	float getMin() const {
		float min = SSE_VEC[0];
		for (int16_t i = 1; i < 4; i++) {
			if (min > SSE_VEC[i])min= SSE_VEC[i];
		}
		return min;
	}
	VEC4 operator +(const VEC4& otherVEC4) const {
		VEC4 v(this->SSE_VEC + otherVEC4.SSE_VEC);
		return v;
	}
	VEC4 operator +(const float& f) const {
		VEC4 v(this->SSE_VEC + (F32vec4)f);
		return v;
	}
	VEC4 operator -(const VEC4& otherVEC4) const {
		VEC4 v(this->SSE_VEC - otherVEC4.SSE_VEC);
		return v;
	}
	VEC4 operator -(const float& f) const {
		VEC4 v(this->SSE_VEC - (F32vec4)f);
		return v;
	}
	VEC4 operator *(const VEC4& otherVEC4) const {
		VEC4 v(this->SSE_VEC * otherVEC4.SSE_VEC);
		return v;
	}
	VEC4 operator *(const float& f) const {
		VEC4 v(this->SSE_VEC * (F32vec4)f);
		return v;
	}
	VEC4 operator /(const VEC4& otherVEC4) const {
		VEC4 v(this->SSE_VEC / otherVEC4.SSE_VEC);
		return v;
	}
	VEC4 operator /(const float& f) const {
		VEC4 v(this->SSE_VEC / (F32vec4)f);
		return v;
	}
	float Length() const{
		F32vec4 SSE_VEC_SQUARED = SSE_VEC * SSE_VEC;
		return sqrtf(SSE_VEC_SQUARED[0] + SSE_VEC_SQUARED[1] + SSE_VEC_SQUARED[2] + SSE_VEC_SQUARED[3]);
	}
};
struct VEC3 {
	F32vec4 SSE_VEC;//using simd for better performance(not using last 32bits)
	VEC3() {
		SSE_VEC[0] = 0;
		SSE_VEC[1] = 0;
		SSE_VEC[2] = 0;
	}
	VEC3(const VEC3& otherVEC3) {
		SSE_VEC = otherVEC3.SSE_VEC;
	}
	VEC3(const F32vec4& otherSSEVEC) {
		SSE_VEC = otherSSEVEC;
	}
	VEC3(const float& x, const float&y, const float& z) {
		SSE_VEC[0] = x;
		SSE_VEC[1] = y;
		SSE_VEC[2] = z;
	}
	VEC3(const float& num) {
		SSE_VEC[0] = num;
		SSE_VEC[1] = num;
		SSE_VEC[2] = num;
	}
	VEC3 Normalize() const{
		VEC3 vec;
		float mag = this->Length();
		vec.SSE_VEC = SSE_VEC / (F32vec4)mag;
		return vec;
	}
	VEC3 Cross(const VEC3& V) const {
		float x1 = SSE_VEC[0], x2 = V.SSE_VEC[0];
		float y1 = SSE_VEC[1], y2 = V.SSE_VEC[1];
		float z1 = SSE_VEC[2], z2 = V.SSE_VEC[2];
		float x = y1 * z2 - y2 * z1;
		float y = z1 * x2 - z2 * x1;
		float z = x1 * y2 - x2 * y1;

		return VEC3(x, y, z);

	}
	float Dot(const VEC3& V)const {
		VEC3 ans = SSE_VEC * V.SSE_VEC;//EXPLICIT CASTING
		return ans.SSE_VEC[0] + ans.SSE_VEC[1] + ans.SSE_VEC[2];
	}
	float getMax() const {
		float max = SSE_VEC[0];
		for (int16_t i = 1; i < 3; i++) {
			if (max < SSE_VEC[i])max = SSE_VEC[i];
		}
		return max;
	}
	float getMin() const {
		float min = SSE_VEC[0];
		for (int16_t i = 1; i < 3; i++) {
			if (min > SSE_VEC[i])min = SSE_VEC[i];
		}
		return min;
	}
	VEC3 operator +(const VEC3& otherVEC3) const {
		VEC3 v(this->SSE_VEC + otherVEC3.SSE_VEC);
		return v;
	}
	VEC3 operator +(const float& f) const {
		VEC3 v(this->SSE_VEC + (F32vec4)f);
		return v;
	}
	VEC3 operator -(const VEC3& otherVEC3) const {
		VEC3 v(this->SSE_VEC - otherVEC3.SSE_VEC);
		return v;
	}
	VEC3 operator -(const float& f) const {
		VEC3 v(this->SSE_VEC - (F32vec4)f);
		return v;
	}
	VEC3 operator *(const VEC3& otherVEC3) const {
		VEC3 v(this->SSE_VEC * otherVEC3.SSE_VEC);
		return v;
	}
	VEC3 operator *(const float& f) const {
		VEC3 v(this->SSE_VEC * (F32vec4)f);
		return v;
	}
	VEC3 operator /(const VEC3& otherVEC3) const {
		VEC3 v(this->SSE_VEC / otherVEC3.SSE_VEC);
		return v;
	}
	VEC3 operator /(const float& f) const {
		VEC3 v(this->SSE_VEC / (F32vec4)f);
		return v;
	}
	float Length() const {
		F32vec4 SSE_VEC_SQUARED = SSE_VEC * SSE_VEC;
		return sqrtf(SSE_VEC_SQUARED[0] + SSE_VEC_SQUARED[1] + SSE_VEC_SQUARED[2]);
	}
};
struct VEC2 {
	F32vec4 SSE_VEC;//using simd for better performance(not using last 64bits)
	VEC2() {
		SSE_VEC[0] = 0;
		SSE_VEC[1] = 0;
	}
	VEC2(const VEC2& otherVEC2) {
		SSE_VEC = otherVEC2.SSE_VEC;
	}
	VEC2(const F32vec4& otherSSEVEC) {
		SSE_VEC = otherSSEVEC;
	}
	VEC2(const float& x, const float& y) {
		SSE_VEC[0] = x;
		SSE_VEC[1] = y;
	}
	VEC2(const float& num) {
		SSE_VEC[0] = num;
		SSE_VEC[1] = num;
	}
	float Dot(const VEC2& V)const {
		VEC2 ans = SSE_VEC * V.SSE_VEC;//EXPLICIT CASTING
		return ans.SSE_VEC[0] + ans.SSE_VEC[1];
	}
	float getMax() const {
		float max = SSE_VEC[0];
			if (max < SSE_VEC[1])max = SSE_VEC[1];
		return max;
	}
	float getMin() const {
		float min = SSE_VEC[0];
			if (min > SSE_VEC[1])min = SSE_VEC[1];
		return min;
	}
	VEC2 operator +(const VEC2& otherVEC2) const {
		VEC2 v(this->SSE_VEC + otherVEC2.SSE_VEC);
		return v;
	}
	VEC2 operator +(const float& f) const {
		VEC2 v(this->SSE_VEC + (F32vec4)f);
		return v;
	}
	VEC2 operator -(const VEC2& otherVEC2) const {
		VEC2 v(this->SSE_VEC - otherVEC2.SSE_VEC);
		return v;
	}
	VEC2 operator -(const float& f) const {
		VEC2 v(this->SSE_VEC - (F32vec4)f);
		return v;
	}
	VEC2 operator *(const VEC2& otherVEC2) const {
		VEC2 v(this->SSE_VEC * otherVEC2.SSE_VEC);
		return v;
	}
	VEC2 operator *(const float& f) const {
		VEC2 v(this->SSE_VEC * (F32vec4)f);
		return v;
	}
	VEC2 operator /(const VEC2& otherVEC2) const {
		VEC2 v(this->SSE_VEC / otherVEC2.SSE_VEC);
		return v;
	}
	VEC2 operator /(const float& f) const {
		VEC2 v(this->SSE_VEC / (F32vec4)f);
		return v;
	}
	float Length() const {
		F32vec4 SSE_VEC_SQUARED = SSE_VEC * SSE_VEC;
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
			rows[i].SSE_VEC[i] = 1;
		}
	}
	MAT4(const MAT4& otherMAT4) {
		for (int16_t i = 0; i < 4; i++) {
			rows[i] = otherMAT4.rows[i];
		}
	}
	MAT4(const float& num) {
		for (int16_t i = 0; i < 4; i++) {
			rows[i].SSE_VEC[0] = num;
			rows[i].SSE_VEC[1] = num;
			rows[i].SSE_VEC[2] = num;
			rows[i].SSE_VEC[3] = num;
		}
	}
	static MAT4 getScrSpaceTransform(const float& halfWidth, const float& halfHeight){
		MAT4 m;
		m.rows[0].SSE_VEC[0] = halfWidth;
		m.rows[1].SSE_VEC[1] = -halfHeight;
		m.rows[0].SSE_VEC[3] = halfWidth;
		m.rows[1].SSE_VEC[3] = halfHeight;
		return m;
	}
	static MAT4 getTranslation(const float& x, const float& y, const float&z){
		MAT4 m;
		m.rows[0].SSE_VEC[3] = x;
		m.rows[1].SSE_VEC[3] = y;
		m.rows[2].SSE_VEC[3] = z;
		return m;
	}
	static MAT4 getRotation(const float& x, const float& y, const float& z, const float& angle){
		float s = (float)sinf(angle);
		float c = (float)cosf(angle);
		float Ic = 1.0f - c;
		MAT4 m;
		m.rows[0].SSE_VEC[0] = c + x * x*Ic; m.rows[0].SSE_VEC[1] = x * y*Ic - z * s; m.rows[0].SSE_VEC[2] = x * z*Ic + y * s;
		m.rows[1].SSE_VEC[0] = y * x*Ic + z * s; m.rows[1].SSE_VEC[1] = c + y * y*Ic; m.rows[1].SSE_VEC[2] = y * z*Ic - x * s;
		m.rows[2].SSE_VEC[0] = z * x*Ic - y * s; m.rows[1].SSE_VEC[1] = z * y*Ic + x * s; m.rows[1].SSE_VEC[2] = c + z * z*Ic;
		return m;

	}
	static MAT4 getRotation(const float& x, const float& y, const float& z){
		MAT4 RX;
		MAT4 RY;
		MAT4 RZ;
		RZ.rows[0].SSE_VEC[0] = (float)cosf(z); RZ.rows[0].SSE_VEC[1] = -sinf(z);
		RZ.rows[1].SSE_VEC[0] = sinf(z); RZ.rows[1].SSE_VEC[1] = cosf(z);		

		RX.rows[1].SSE_VEC[1] = cosf(x); RX.rows[1].SSE_VEC[2] = -sinf(x); 
		RX.rows[2].SSE_VEC[1] = sinf(x); RX.rows[2].SSE_VEC[2] = cosf(x);

		RY.rows[0].SSE_VEC[0] = cosf(y); RY.rows[0].SSE_VEC[2] = -sinf(y);
		RY.rows[2].SSE_VEC[0] = sinf(y); RY.rows[2].SSE_VEC[2] = cosf(y);
	
		
		return  RZ * RY * RX;

	}
	static MAT4 getScale(const float& x, const float& y, const float& z)
	{
		MAT4 m;
		m.rows[0].SSE_VEC[0] = x;m.rows[1].SSE_VEC[1] = y;	m.rows[2].SSE_VEC[2] = z;
		return m;
	}

	static MAT4 getPerspective(const float& FOV,const float& aspectRatio,const float& zNear,const float& zFar)
	{
		float tanHalfFOV =tanf((FOV / 2.0f)*(3.14159265359f/180.0f));
		float zRange = zNear - zFar;
		MAT4 m;
		m.rows[0].SSE_VEC[0] = 1.0f / (tanHalfFOV * aspectRatio);
		m.rows[1].SSE_VEC[1] = 1.0f / tanHalfFOV;
		m.rows[2].SSE_VEC[2] = (-zNear - zFar) / zRange;
		m.rows[2].SSE_VEC[3] = 2 * zFar * zNear / zRange;
		m.rows[3].SSE_VEC[2] = 1.0f;
		m.rows[3].SSE_VEC[3] = 0.0f;
		
		return m;
	}

	static MAT4 getOrthographic(const float& left, const float& right, const float& bottom, const float& top, const float& near, const float& far)
	{
		float width = right - left;
		float height = top - bottom;
		float depth = far - near;
		MAT4 m;
		m.rows[0].SSE_VEC[0] = 2 / width;	
		m.rows[0].SSE_VEC[3] = -(right + left) / width;
		m.rows[1].SSE_VEC[1] = 2 / height;
		m.rows[1].SSE_VEC[3] = -(top + bottom) / height;
		m.rows[2].SSE_VEC[2] = -2 / depth;
		m.rows[2].SSE_VEC[3] = -(far + near) / depth;

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
		m.rows[0].SSE_VEC[0] = r.SSE_VEC[0];	m.rows[0].SSE_VEC[1] = r.SSE_VEC[1];	m.rows[0].SSE_VEC[2] = r.SSE_VEC[2];
		m.rows[1].SSE_VEC[0] = u.SSE_VEC[0];	m.rows[1].SSE_VEC[1] = u.SSE_VEC[1];	m.rows[1].SSE_VEC[2] = u.SSE_VEC[2];	
		m.rows[2].SSE_VEC[0] = f.SSE_VEC[0];	m.rows[2].SSE_VEC[1] = f.SSE_VEC[1];	m.rows[2].SSE_VEC[2] = f.SSE_VEC[2];			

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
			tmp.SSE_VEC[0] = otherMAT4.rows[0].SSE_VEC[i];
			tmp.SSE_VEC[1] = otherMAT4.rows[1].SSE_VEC[i];
			tmp.SSE_VEC[2] = otherMAT4.rows[2].SSE_VEC[i];
			tmp.SSE_VEC[3] = otherMAT4.rows[3].SSE_VEC[i];
			m.rows[0].SSE_VEC[i] = rows[0].Dot(tmp);
			m.rows[1].SSE_VEC[i] = rows[1].Dot(tmp);
			m.rows[2].SSE_VEC[i] = rows[2].Dot(tmp);
			m.rows[3].SSE_VEC[i] = rows[3].Dot(tmp);
		}
		return m;
	}

};
struct Vertex {
	VEC4 position;
	Color c;
	Vertex(float x, float y,float z, uint32_t col) {
		position = VEC4(x, y,z,1);
		c = Color(col);
	}
	Vertex(const VEC4& pos,const Color& col) {
		position = VEC4(pos);
		c = Color(col);
	}
	Vertex(const VEC4& pos) {
		position = VEC4(pos);
		c = Color(0xffffffff);
	}
	Vertex Transform(const MAT4& transMat) const
	{
		return Vertex(transMat.Transform(position), this->c);
	}

	Vertex PrespDivide() const
	{
		return Vertex(VEC4(position.SSE_VEC[0] / position.SSE_VEC[3], position.SSE_VEC[1] / position.SSE_VEC[3],
			position.SSE_VEC[2] / position.SSE_VEC[3], position.SSE_VEC[3]),this->c);
	}

};
