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
}

void Mesh::draw(){
    int count =0;
    for (auto& tri: triangles){
        count++;
        for (int i=0;i<3; i++){
            std::cout << tri.vertices[i][0] <<"\t"<< tri.vertices[i][1] <<"\t"<< tri.vertices[i][2]<< "\t";
        }
        std::cout <<"\n";
        tri.wireframe_draw();
        tri.draw();
    }
    std::cout << count;
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

void Mesh::perspective(){
    float zprp= 250, xprp = 0, yprp = 0;
    float zvp=0;
    float dp = zprp - zvp;
    maths::mat4f persmatrix = {{{1,0,xprp/dp,-xprp*zvp/dp},
                            {0,1,yprp/dp,-yprp*zvp/dp},
                            {0,0,-zvp/dp,zvp*(zprp/dp)},
                            {0,0,-1/dp,zprp/dp}}};               
    for (auto& tri: triangles){
        tri.vertices[0] = maths::mul(persmatrix,tri.vertices[0]);
        tri.vertices[1] = maths::mul(persmatrix, tri.vertices[1]);
        tri.vertices[2] = maths::mul(persmatrix, tri.vertices[2]);
    }
}
