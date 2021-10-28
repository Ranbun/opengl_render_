#ifndef  VERTEXARRAYBUFFEROBJECT_H
#define  VERTEXARRAYBUFFEROBJECT_H

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();
	void bind() const;
	bool create();
	void destroy();
	bool isCreated() const;
	unsigned int objectId() const ;
	static void release();
	

private:
	unsigned int vao_;
	bool is_create_;
};


#endif 