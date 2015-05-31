#ifndef METNUM_TP3_MATRIX_H
#define METNUM_TP3_MATRIX_H

#include <iostream>
#include <bitset>
#include <vector>

/*
* Matriz.
*/
class Matrix {
    friend std::ostream &operator<<(std::ostream &, const Matrix &);
    friend std::istream &operator>>(std::istream &, Matrix &);
public:
    Matrix(const Matrix &m);

    Matrix(Matrix &&m) : N(m.rows()), M(m.columns()), matrix(std::move(m.matrix)) {
        std::cerr << "Llamado al constructor por movimiento de Matriz " << this->rows() << "x" << this->columns() << std::endl;

        if (this->matrix.size() > 0) {
            std::cerr << "Dimensiones del vector de salida: " << this->matrix.size() << "x" << this->matrix[0].size() << std::endl;
        }
    }

    Matrix(Matrix& m, int k) : N(m.rows() + (k* (m.rows()-1))), M(m.columns() + (k* (m.columns()-1))), matrix(N, std::vector<int>(M, 0.0))
    {
        int ki=0;
        for(int i =0;i<m.rows();i++)
        {
            int kj=0;
            for(int j=0;j<m.columns();j++)
            {
                this->matrix[ki][kj]= m(i,j);
                kj=kj+k+1;
            }
            ki=ki+k+1;
        }
    }

    /*template <std::size_t K>
    Matrix(const Matrix &m, const std::bitset<K> &filter)
            : N((int)filter.count()), M(m.columns()), matrix((int)filter.count(), std::vector<double>(m.columns(), 0.0)) {
        if (K != (std::size_t)m.rows()) {
            throw new std::out_of_range("Filtro de bitset para Matriz con entradas insuficientes");
        }

        if (this->rows() < 0 || this->columns() < 0) {
            throw new std::invalid_argument("Dimensiones de Matriz inválidas");
        }

        std::cerr << "Filtrando matriz de " << m.rows() << "x" << m.columns() << " en " << this->rows() << "x" << this->columns() << std::endl;

        int last = 0;

        for (int i = 0; i < m.rows(); ++i) {
            if (filter.test((std::size_t) i)) {
                std::copy(m.matrix[i].begin(), m.matrix[i].begin() + m.columns(), this->matrix[last].begin());
                last++;
            }
        }

        if (this->matrix.size() > 0) {
            std::cerr << "Dimensiones del vector de salida: " << this->matrix.size() << "x" << this->matrix[0].size() << std::endl;
        }
    }*/

    Matrix(int N, int M);

    int inline rows() const {
        return this->N;
    }

    int inline columns() const {
        return this->M;
    }

    inline int &operator()(const int &i, const int &j) {
        #ifdef DEBUG
        if (0 > i || 0 > j || i >= this->rows() || j >= this->columns()) {
            throw new std::out_of_range("Index access out of range");
        }
        #endif

        return this->matrix[i][j];
    }

    inline const int &operator()(const int &i, const int &j) const {
        #ifdef DEBUG
        if (0 > i || 0 > j || i >= this->rows() || j >= this->columns()) {
            throw new std::out_of_range("Index access out of range");
        }
        #endif

        return this->matrix[i][j];
    }

    void writeMatrix(const std::string& s);


    Matrix & operator=(const Matrix &m);
    bool operator==(const Matrix &m) const;
    bool operator!=(const Matrix &m) const;
    Matrix & operator+=(const Matrix &m);
    Matrix & operator*=(const int &c);
private:
    // Matrix
    int N;
    int M;
    std::vector< std::vector<int> > matrix;
};

std::ostream &operator<<(std::ostream &, const Matrix &);
std::istream &operator>>(std::istream &, Matrix &);
Matrix operator+(const Matrix &m, const Matrix &n);
Matrix operator*(const Matrix &m, const int &c);
Matrix operator*(const Matrix &m, const Matrix &n);

#endif //METNUM_TP3_MATRIX_H
