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
        // tri.rasterize();
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
    int counter =0 ;
    for (auto& tri: finalTris){
        counter++;
        phongIlluminationModel(tri);
        // std::cout << "colors\n";

    }
    std::cout << "The counter is: "<< counter <<"\n" ;
}

void Mesh::backFaceCulling(Triangle& tri){
    // for(auto& triangle : cube.triangles){
    maths::vec3f v1 = tri.vertices[0];
    maths::vec3f v2 = tri.vertices[1];
    maths::vec3f v3 = tri.vertices[2];

    maths::vec3f centroid;
    centroid[0] = (v1[0] + v2[0] + v3[0]) / 3; 
    centroid[1] = (v1[1] + v2[1] + v3[1]) / 3; 
    centroid[2] = (v1[2] + v2[2] + v3[2]) / 3;

    maths::vec3f v =maths::normalize(maths::sub(camera->Position,centroid));

    // generating the normal vector of a triangle
    maths::vec3f ver1 = maths::sub(tri.vertices[0],tri.vertices[1]);
    maths::vec3f ver2 = maths::sub(tri.vertices[0],tri.vertices[2]);

    maths::vec3f normal =maths::normalize(maths::cross(ver1,ver2));
    float dotProduct = maths::dot(normal,v);
    if(dotProduct < 0){
        finalTris.push_back(tri);
    }
}

float Mesh::calculateIntensity(maths::vec3f point, maths::vec3f Normal, maths::vec3f View,float specularExp){
    float i = 0.0;
    maths::vec3f position = {500,1000,800};
    maths::vec3f Ldir = maths::normalize(maths::sub(position, point));
    std::cout << point[0] << "\t" << point[1] << "\t" << point[2] << "\n";
    float ambientInt = 0.4;
    float pointInt = 0.5;

    float ambientConstant = 0.8;
    float diffuseConstant = 0.7;
    float specularConstant = 0.8;

    float ambientLight = ambientConstant*ambientInt;
    
    float diffuseLight = diffuseConstant* pointInt *maths::dot(Normal,Ldir);

    maths::vec3f R = maths::sub(maths::mul(Normal,(2* maths::dot(Normal,Ldir))),Ldir);
    float specularLight = specularConstant * pointInt * pow(maths::dot(R,View),specularExp);
    
    float tmp = ambientLight+specularLight+diffuseLight;
    tmp = tmp > 1 ? 1: tmp;
    return tmp;
}


void Mesh::phongIlluminationModel(Triangle& tri){
    // for (auto &triangle : cube.triangles)
    // {
        maths::vec3f v1 = tri.vertices[0];
        maths::vec3f v2 = tri.vertices[1];
        maths::vec3f v3 = tri.vertices[2];

        maths::vec3f centroid;
        centroid[0] = (v1[0] + v2[0] + v3[0]) / 3; 
        centroid[1] = (v1[1] + v2[1] + v3[1]) / 3; 
        centroid[2] = (v1[2] + v2[2] + v3[2]) / 3; 

        // std::cout << centroid[0] <<"\t";

        maths::vec3f view = maths::normalize(maths::sub(camera->Position,centroid));
        
        // generating the normal vector of a triangle
        maths::vec3f ver1 = maths::sub(v1,v2);
        maths::vec3f ver2 = maths::sub(v1,v3);

        maths::vec3f normal = maths::normalize(maths::cross(ver1,ver2));

        float intensity = calculateIntensity(centroid,normal,view,10);
        std::cout << "The intensity: " << intensity <<"\n";
        maths::vec3f newColor = maths::mul(tri.color,intensity);

        tri.color = newColor;
        // triangle.print();
}
    
