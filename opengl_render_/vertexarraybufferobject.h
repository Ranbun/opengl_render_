#ifndef  VERTEXARRAYBUFFEROBJECT_H_
#define  VERTEXARRAYBUFFEROBJECT_H_

class VertexArrayObject
{
public:
    VertexArrayObject();
    ~VertexArrayObject();
    VertexArrayObject(const VertexArrayObject& another);
    VertexArrayObject(VertexArrayObject&& another) noexcept;

    void bind() const;
    bool create();
    void destroy();
    bool isCreated() const;
    unsigned int objectId() const;
    void release();

private:
    unsigned int m_arrayBuffer;
    bool m_isCreate;
};


#endif 