#pragma once
#include "Vec4f.h"
class Mat4x4f
{
public:
	inline Mat4x4f() {
		//instantiate identity matrix
		for (int32_t i = 0; i < 4; i++) {
			rows[i] = Vec4f(); rows[i].insert(1.0f, i);
		}
	};
	inline Mat4x4f(const Mat4x4f& otherMat) { 
		for (int32_t i = 0; i < 4; i++) {
			this->rows[i] = otherMat[i];
		}
	}
	inline __m128 getVec(int32_t idx)const { return rows[idx].getVec(); }
	inline void setVec(const Vec4f& v, int32_t idx) { rows[idx] = v; }
	inline Vec4f Transform(const Vec4f& r) const
	{
		return Vec4f(dot(rows[0], r), dot(rows[1], r), dot(rows[2], r), dot(rows[3], r));
	}
	inline Mat4x4f operator *(const Mat4x4f& otherMat) {
		Mat4x4f m;
		Vec4f tmp;
		for (int32_t i = 0; i < 4; i++) {//iterate through other mat cols
			tmp.insert(otherMat[0][i], 0);
			tmp.insert(otherMat[1][i], 1);
			tmp.insert(otherMat[2][i], 2);
			tmp.insert(otherMat[3][i], 3);
			m.getRow(0)->insert(dot(rows[0], tmp), i);
			m.getRow(1)->insert(dot(rows[1], tmp), i);
			m.getRow(2)->insert(dot(rows[2], tmp), i);
			m.getRow(3)->insert(dot(rows[3], tmp), i);
		}
		return m;
	}
	inline Vec4f* getRow(int32_t idx) { return &rows[idx]; }
	inline Vec4f operator[](int32_t idx) const { return rows[idx]; }
	inline Mat4x4f Transposed() const {
		Mat4x4f m;
		m.getRow(0)->setVec(rows[0][0], rows[1][0], rows[2][0], rows[3][0]);
		m.getRow(1)->setVec(rows[0][1], rows[1][1], rows[2][1], rows[3][1]);
		m.getRow(2)->setVec(rows[0][2], rows[1][2], rows[2][2], rows[3][2]);
		m.getRow(3)->setVec(rows[0][3], rows[1][3], rows[2][3], rows[3][3]);
		return m;
	}
	static inline Mat4x4f getScrSpaceTransform(const float& halfWidth, const float& halfHeight) {
		Mat4x4f m;
		m.getRow(0)->insert(halfWidth, 0);
		m.getRow(1)->insert(-halfHeight, 1);
		m.getRow(0)->insert(halfWidth, 3);
		m.getRow(1)->insert(halfHeight, 3);
		return m;
	}
	static inline Mat4x4f getTranslation(const float& x, const float& y, const float&z) {
		Mat4x4f m;
		m.getRow(0)->insert(x, 3);
		m.getRow(1)->insert(y, 3);
		m.getRow(2)->insert(z, 3);
		return m;
	}
	static inline Mat4x4f getRotation(const float& x, const float& y, const float& z) {
		Mat4x4f RX;
		Mat4x4f RY;
		Mat4x4f RZ;
		RZ.getRow(0)->setVec(cos(z), -sin(z), 0.0f, 0.0f);
		RZ.getRow(1)->setVec(sin(z), cos(z), 0.0f, 0.0f);

		RY.getRow(0)->setVec(cos(y), 0.0f, -sin(y), 0.0f);
		RY.getRow(2)->setVec(sin(y), 0.0f, cos(y), 0.0f);

		RX.getRow(1)->setVec(0.0f, cos(x), -sin(x), 0.0f);
		RX.getRow(2)->setVec(0.0f, sin(x), cos(x), 0.0f);
		return  RZ * RY * RX;

	}
	static inline Mat4x4f getScale(const float& x, const float& y, const float& z)
	{
		Mat4x4f m;
		m.getRow(0)->insert(x, 0);
		m.getRow(1)->insert(y, 1);
		m.getRow(2)->insert(z, 2);
		return m;
	}
	static inline Mat4x4f getPerspective(const float& FOV, const float& aspectRatio, const float& zNear, const float& zFar)
	{
		float tanHalfFOV = tanf((FOV / 2.0f));
		float zRange = zNear - zFar;
		Mat4x4f m;
		m.getRow(0)->insert(1.0f / (tanHalfFOV * aspectRatio) , 0);
		m.getRow(1)->insert(1.0f / tanHalfFOV, 1);
		m.getRow(2)->insert((-zNear - zFar) / zRange, 2);
		m.getRow(2)->insert(2 * zFar * zNear / zRange, 3);
		m.getRow(3)->insert(1.0f, 2);
		m.getRow(3)->insert(0.0f, 3);
		return m;
	}
	static inline Mat4x4f getOrthographic(const float& left, const float& right, const float& bottom, const float& top, const float& near, const float& far)
	{
		float width = right - left;
		float height = top - bottom;
		float depth = far - near;
		Mat4x4f m;
		m.getRow(0)->insert(2 / width, 0);
		m.getRow(0)->insert(-(right + left) / width, 3);
		m.getRow(1)->insert(2 / height, 1);
		m.getRow(1)->insert(-(top + bottom) / height, 3);
		m.getRow(2)->insert(-2 / depth, 2);
		m.getRow(2)->insert(-(far + near) / depth, 3);
		return m;
	}
	static inline Mat4x4f getLookAt(const Vec4f& eye, const Vec4f& center, const Vec4f& up) {
		Mat4x4f m;
		Vec4f R, U, D;
		D = (eye - center).normalized();
		R = cross(up, D).normalized();
		U = cross(D, R).normalized();
		m.getRow(0)->setVec(R[0], R[1], R[2], -dot(R, eye));
		m.getRow(1)->setVec(U[0], U[1], U[2], -dot(U, eye));
		m.getRow(2)->setVec(D[0], D[1], D[2], -dot(D, eye));
		m.getRow(3)->setVec(0.0f,0.0f ,0.0f , 1.0f);
		return m;
		//Mat4x4f Mr, Mt;
		//Vec4f D = (eye - center).normalized();
		//Vec4f R = cross(up, D).normalized();
		//Vec4f U = cross(D, R);
		//Mr.setVec(R, 0);
		//Mr.setVec(U, 1);
		//Mr.setVec(D, 2);
		//Mr.getRow(3)->setVec(   0.0f,  0.0f,       0.0f, 1.0f);
		//Mt = getTranslation(-eye.getX(), -eye.getY(), -eye.getZ());
		//return Mr * Mt;
	}
	inline ~Mat4x4f() { };

private:
	Vec4f rows[4];

};

//Opertions
static inline Mat4x4f operator +(const Mat4x4f& m1, const Mat4x4f& m2) {
	Mat4x4f m;
	for (int32_t i = 0; i < 4; i++) {
		m[i] = m1[i] + m2[i];
	}
	return m;
}

static inline Mat4x4f operator -(const Mat4x4f& m1, const Mat4x4f& m2) {
	Mat4x4f m;
	for (int32_t i = 0; i < 4; i++) {
		m[i] = m1[i] - m2[i];
	}
	return m;
}

static inline Mat4x4f operator *(const Mat4x4f& m1, const Mat4x4f& m2) {
	Mat4x4f m;
	Vec4f tmp;
	for (int32_t i = 0; i < 4; i++) {//iterate through other mat cols
		tmp.insert( m2[0][i],0);
		tmp.insert( m2[1][i],1);
		tmp.insert( m2[2][i],2);
		tmp.insert( m2[3][i],3);
		m.getRow(0)->insert(dot(m1[0], tmp), i);
		m.getRow(1)->insert(dot(m1[1], tmp), i);
		m.getRow(2)->insert(dot(m1[2], tmp), i);
		m.getRow(3)->insert(dot(m1[3], tmp), i);
	}
	return m;
}
static inline std::ostream& operator << (std::ostream& output, const Mat4x4f& m) {	output << "Matrix4x4f: \n";for (int i = 0; i < (4); ++i)output << m[i] << "\n";	return output;}

