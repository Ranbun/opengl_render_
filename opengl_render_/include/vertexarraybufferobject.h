#ifndef  VERTEX_ARRAY_BUFFER_OBJECT_H_
#define  VERTEX_ARRAY_BUFFER_OBJECT_H_

class VertexArrayObject
{
public:
    VertexArrayObject();
    ~VertexArrayObject();

    VertexArrayObject(const VertexArrayObject& another) = delete;
    VertexArrayObject(const VertexArrayObject&& another) = delete;
    VertexArrayObject& operator=(const VertexArrayObject& another) = delete;
    VertexArrayObject& operator=(VertexArrayObject&& another) = delete;

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