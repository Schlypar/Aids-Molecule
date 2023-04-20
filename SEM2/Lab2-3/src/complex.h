#pragma once

#include <iostream>
#include <utility>
#include <cmath>

class complex
{
private:
    float real;
    float imaginary;

public:
    complex()
        : real(0), imaginary(0)
    {
    }

    complex(const float& real)
        : real(real), imaginary()
    {
    }

    complex(float&& real)
        : real(std::move(real)), imaginary()
    {
        real = float();
    }

    complex(const float& real, const float& imaginary)
        : real(real), imaginary(imaginary)
    {
    }

    complex(float&& real, float&& imaginary)
        : real(std::move(real)), imaginary(std::move(imaginary))
    {
        real = float();
        imaginary = float();
    }

    complex(const complex& other)
        : real(other.real), imaginary(other.imaginary)
    {
    }

    complex(complex&& other)
        : real(std::move(other.real)), imaginary(std::move(other.imaginary))
    {
        other.real = float();
        other.imaginary = float();
    }

    complex& operator+ (const float& other)
    {
        real += other;

        return *this;
    }

    friend complex& operator+ (const float& other, complex& complex)
    {
        return complex + other;
    }

    friend complex& operator+ (const float& other, complex&& complex)
    {
        return complex + other;
    }

    complex& operator+ (const complex& other)
    {
        real += other.real;
        imaginary += other.imaginary;

        return *this;
    }

    complex& operator+= (const float& other)
    {
        real += other;

        return *this;
    }

    complex& operator+= (const complex& other)
    {
        real += other.real;
        imaginary += other.imaginary;

        return *this;
    }

    complex& operator+= (complex&& other)
    {
        real += other.real;
        imaginary += other.imaginary;

        return *this;
    }

    complex& operator- (const float& other)
    {
        real -= other;

        return *this;
    }

    complex& operator- (const complex& other)
    {
        real -= other.real;
        imaginary -= other.imaginary;

        return *this;
    }
    
    friend complex& operator- (const float& other, complex& complex)
    {
        return complex - other;
    }

    friend complex& operator- (const float& other, complex&& complex)
    {
        return complex - other;
    }  

    friend complex& operator- (complex& complex)
    {
        complex.real = -complex.real;
        complex.imaginary = -complex.imaginary;

        return complex;
    }

    complex& operator-= (const float& other)
    {
        real -= other;

        return *this;
    }

    complex& operator-= (const complex& other)
    {
        real -= other.real;
        imaginary -= other.imaginary;

        return *this;
    }

    complex& operator-= (complex&& other)
    {
        real -= other.real;
        imaginary -= other.imaginary;

        return *this;
    }

    complex& operator* (const float& other)
    {
        real = real * other;

        return *this;
    }

    friend complex& operator* (const float& other, complex& complex)
    {
        return complex * other;
    }

    friend complex& operator* (const float& other, complex&& complex)
    {
        return complex * other;
    }

    complex& operator* (const complex& other)
    {
        real = real * other.real - imaginary * other.imaginary;
        imaginary = real * other.imaginary + other.real * imaginary;

        return *this;
    }

    complex& operator/ (const float& other)
    {
        real = real / other;
        imaginary = imaginary / other;

        return *this;
    }

    complex& operator/ (const complex& other)
    {
        real = (real * other.real + imaginary * other.imaginary) / (other.real * other.real + other.imaginary * other.imaginary);
        imaginary = (imaginary * other.real - real * other.imaginary) / (other.real * other.real + other.imaginary * other.imaginary);

        return *this;
    }

    friend complex& operator/ (const float& other, complex& _complex)
    {
        return complex(other) / _complex;
    }

    friend complex& operator/ (const float& other, complex&& _complex)
    {
        return complex(other) / _complex;
    }

    complex& operator= (const float& other) 
    {
        real = other;
        imaginary = 0;

        return *this;
    }

    complex& operator= (const complex& other) 
    {
        real = other.real;
        imaginary = other.imaginary;

        return *this;
    }

    complex& operator= (float&& other) 
    {
        real = std::move(other);
        imaginary = 0;

        other = float();

        return *this;
    }

    complex& operator= (complex&& other) 
    {
        real = other.real;
        imaginary = other.imaginary;

        other = 0;

        return *this;
    }

    bool operator== (const float& other)
    {
        return (std::sqrt(real * real + imaginary * imaginary) == other) ? true : false;
    }

    friend bool operator== (const float& other, complex& complex)
    {
        return complex == other;
    }

    friend bool operator== (const float& other, complex&& complex)
    {
        return complex == other;
    }

    bool operator== (const complex& other)
    {
        return (std::sqrt(real * real + imaginary * imaginary) == std::sqrt(other.real * other.real + other.imaginary * other.imaginary)) ? true : false;
    }

    bool operator!= (const float& other)
    {
        return !(*this == other);
    }

    friend bool operator!= (const float& other, complex& complex)
    {
        return complex != other;
    }

    friend bool operator!= (const float& other, complex&& complex)
    {
        return complex != other;
    }

    bool operator!= (const complex& other)
    {
        return !(*this == other);
    }

    bool operator< (const float& other)
    {
        return (std::sqrt(real * real + imaginary * imaginary) < other) ? true : false;
    }

    friend bool operator< (const float& other, complex& complex)
    {
        return complex > other;
    }

    friend bool operator< (const float& other, complex&& complex)
    {
        return complex > other;
    }

    bool operator< (const complex& other)
    {
        return (std::sqrt(real * real + imaginary * imaginary) < std::sqrt(other.real * other.real + other.imaginary * other.imaginary)) ? true : false;
    }

    bool operator> (const float& other)
    {
        return !(*this < other);
    }

    friend bool operator> (const float& other, complex& complex)
    {
        return complex < other;
    }

    friend bool operator> (const float& other, complex&& complex)
    {
        return complex < other;
    }

    bool operator> (const complex& other)
    {
        return !(*this < other);
    }

    bool operator<= (const float& other)
    {
        if (*this == other || *this < other)
            return true;
        else 
            return false;
    }

    friend bool operator<= (const float& other, complex& complex)
    {
        return complex >= other;
    }

    friend bool operator<= (const float& other, complex&& complex)
    {
        return complex >= other;
    }

    bool operator<= (const complex& other)
    {
        if (*this == other || *this < other)
            return true;
        else 
            return false;
    }

    bool operator>= (const float& other)
    {
        if (*this == other || *this > other)
            return true;
        else 
            return false;
    }

    friend bool operator>= (const float& other, complex& complex)
    {
        return complex <= other;
    }

    friend bool operator>= (const float& other, complex&& complex)
    {
        return complex <= other;
    }

    bool operator>= (const complex& other)
    {
        if (*this == other || *this > other)
            return true;
        else 
            return false;
    }

    friend std::ostream& operator<< (std::ostream& stream, const complex& complex)
    {
        if (complex.imaginary == 0)
            stream << complex.real;
        else if (complex.imaginary >= 0)
            stream << complex.real << " + " << complex.imaginary << 'i';
        else
            stream << complex.real << " - " << -complex.imaginary << 'i';

        return stream; 
    } 
};