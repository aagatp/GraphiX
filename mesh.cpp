#include "mesh.h"

Mesh::Mesh(Canvas* mcanvas){
    canvas = mcanvas;
}

void Mesh::load(std::string filename){
    std::ifstream file;
    file.open(filename);
    if (file.fail()){
        std::cout << "File cannot be opened \n";
        exit(-1);
    }
		// Local cache of verts
	std::vector<maths::vec3f> verts;

    while (!file.eof())
    {
        char line[128];
        file.getline(line, 128);

        std::stringstream s;
        s << line;

        char junk;

        if (line[0] == 'v')
        {
            maths::vec3f v;
            s >> junk >> v[0] >> v[1] >> v[2];
            verts.push_back(v);
        }

        if (line[0] == 'f')
        {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            triangles.push_back(Triangle{canvas,verts[f[0] - 1],verts[f[1] - 1],verts[f[2] - 1]});
        }
    }
    trisViewProject = triangles;
}

void Mesh::draw(){
    int count =0;
    std::vector<Triangle> temptris = isViewProject ? finalTris : triangles;
    for (auto& tri: temptris){
        count++;
        tri.wireframe_draw();
        // tri.draw();
    }
    // std::cout << count;
}

void Mesh::translate(float tx, float ty, float tz){
    for (auto& tri: triangles){
        maths::vec3f a = maths::mul(maths::translate(tx,ty,tz), tri.vertices[0]);
        maths::vec3f b = maths::mul(maths::translate(tx,ty,tz), tri.vertices[1]);
        maths::vec3f c = maths::mul(maths::translate(tx,ty,tz), tri.vertices[2]); 
        tri.vertices = {a,b,c};
    }
}

void Mesh::rotate(float yaw, float pitch=0, float roll=0){
    for (auto& tri: triangles){
        tri.vertices[0] = maths::mul(maths::rotate(yaw,pitch,roll), tri.vertices[0]);
        tri.vertices[1] = maths::mul(maths::rotate(yaw,pitch,roll), tri.vertices[1]);
        tri.vertices[2] = maths::mul(maths::rotate(yaw,pitch,roll), tri.vertices[2]);
    }
}

void Mesh::scale(float sx, float sy, float sz){
    for (auto& tri: triangles){
        tri.vertices[0] = maths::mul(maths::scale(sx,sy,sz), tri.vertices[0]);
        tri.vertices[1] = maths::mul(maths::scale(sx,sy,sz), tri.vertices[1]);
        tri.vertices[2] = maths::mul(maths::scale(sx,sy,sz), tri.vertices[2]);
    }
}

void Mesh::applyTransform(maths::mat4f& transform){
    isViewProject = true;
    finalTris.clear();
    int count = 0;
    for (auto& tri: triangles){
        // Triangle tmptri();
        trisViewProject[count].vertices[0] = maths::mul(transform, tri.vertices[0]);
        trisViewProject[count].vertices[1] = maths::mul(transform, tri.vertices[1]);
        trisViewProject[count].vertices[2] = maths::mul(transform, tri.vertices[2]);
        count++;
        
        backFaceCulling(trisViewProject[count]);
    }
}

void Mesh::backFaceCulling(Triangle& tri){
    // for(auto& triangle : cube.triangles){

    maths::vec3f v = maths::sub(camera->Position,tri.vertices[2]);

    // generating the normal vector of a triangle
    maths::vec3f v1 = maths::sub(tri.vertices[1],tri.vertices[0]);
    maths::vec3f v2 = maths::sub(tri.vertices[2],tri.vertices[0]);

    maths::vec3f normal = maths::cross(v1,v2);
    // normal.print();
    float dotProduct = maths::dot(normal,v);
    // std::cout<<dotProduct<<std::endl;
    if(dotProduct >= 0){
        finalTris.push_back(tri);
    }
}