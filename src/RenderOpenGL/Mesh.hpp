#pragma once

#include <memory>
#include <vector>

#include <IVertexLayoutDescriptor.hpp>

namespace Snowglobe::RenderOpenGL
{
    template <typename T>
    class Mesh
    {
    public:
        static Mesh<T>* Create(const IVertexLayoutDescriptor& layoutDescriptor, std::unique_ptr<std::vector<T>> vertices, std::unique_ptr<std::vector<unsigned int>> indices)
        {
            if(!layoutDescriptor.ValidateVertexType(typeid(T)))
            {
                std::cout << "Invalid vertex type" << std::endl;
                return nullptr;
            }

            return new Mesh(layoutDescriptor, std::move(vertices), std::move(indices));
        }

        ~Mesh() 
        {
            glDeleteVertexArrays(1, &_vao);
            glDeleteBuffers(1, &_vbo);
            glDeleteBuffers(1, &_ebo);
        }

        void Bind() const { glBindVertexArray(_vao); }
        void Unbind() const { glBindVertexArray(0); }
    
    private:
        Mesh(const IVertexLayoutDescriptor& layoutDescriptor, std::unique_ptr<std::vector<T>> vertices, std::unique_ptr<std::vector<unsigned int>> indices) 
            : _vertexBuffer(std::move(vertices)), _indexBuffer(std::move(indices))
        {
            glGenVertexArrays(1, &_vao);
            glGenBuffers(1, &_vbo);
            glGenBuffers(1, &_ebo);

            glBindVertexArray(_vao);

            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, _vertexBuffer->size() * sizeof(T), _vertexBuffer->data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer->size() * sizeof(unsigned int), _indexBuffer->data(), GL_STATIC_DRAW);

            layoutDescriptor.SetupAtributePointers();

            glBindVertexArray(0);
        }

        std::unique_ptr<std::vector<T>> _vertexBuffer;
        std::unique_ptr<std::vector<unsigned int>> _indexBuffer;

        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ebo;
    };

} // namespace Snowglobe::RenderOpenGL
