#include "mesh.h"
#include <algorithm>

Mesh::Mesh(Canvas* mcanvas){
    canvas = mcanvas;
    projection = maths::matidentity();
    view = maths::matidentity();
    isWireframe = true;
    isGouraudShade = false;
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
    finalTris = triangles;

}

void Mesh::parse(std::string filename){
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
    {
        std::cout << "Cannot Read" << std::endl;
        exit(-1);
    }
    std::string line;
    std::vector<maths::vec3f> verts;
    std::vector<maths::vec3f> normals;
    std::vector<maths::vec2f> textures;
    
    int count =1;
    while (!in.eof())
    {
        //get one line at a time
        std::getline(in, line);
        //string object
        std::istringstream iss(line.c_str());

        char trash;
        if (!line.compare(0, 2, "v "))  //starts with v<space>
        {   

            // std::cout << count;           
            iss >> trash; // first character is v
            maths::vec3f v;
            // followed by xyz co-ords
            iss >> v[0];
            iss >> v[1];
            iss >> v[2];
            // maths::printvec(v);
            verts.push_back(v);
            count++;
        }
        else if (!line.compare(0, 3, "vt "))    //starts with vt<space>
        {
            iss >> trash >> trash;//Ignore vt
            maths::vec2f uv;
            iss >> uv[0];
            iss >> uv[1];
            textures.push_back(uv);
        }

        else if (!line.compare(0, 3, "vn "))    //starts with vn<space>
        {
            iss >> trash >> trash;
            maths::vec3f n;
            iss >> n[0];
            iss >> n[1];
            iss >> n[2];
            normals.push_back(n);
        }
        else if (!line.compare(0, 2, "f ")) //starts with f<space>
        {
            std::vector<maths::vec3i> f;
            maths::vec3i temp;

            iss >> trash; //first charecter is f

            while (iss >> temp[0] >> trash >> temp[1] >> trash >> temp[2])
            // in the form vert/vertTex/norm (vert is read, the rest are treated as trash)
            {
                //in wavefront obj all indices start at 1, not zero
                temp[0]--;   //vert
                temp[1]--;   //texture
                temp[2]--; // normal 
                f.push_back(temp);
            }
            Triangle tri(canvas);
            tri.setVertex(verts[f[0][0]],verts[f[1][0]],verts[f[2][0]]);
            // std::cout  << f[0][0] <<'\n';       
            tri.setTexCoords(textures[f[0][1]],textures[f[1][1]],textures[f[2][1]]);
            tri.setNormals(normals[f[0][2]],normals[f[1][2]],normals[f[2][2]]);
            triangles.push_back(tri);
        }
    }   
    finalTris = triangles;
}


void Mesh::translate(float tx, float ty, float tz){
    for (auto& tri: triangles){
        maths::vec3f a = maths::mul(maths::translate(tx,ty,tz), tri.vertices[0]);
        maths::vec3f b = maths::mul(maths::translate(tx,ty,tz), tri.vertices[1]);
        maths::vec3f c = maths::mul(maths::translate(tx,ty,tz), tri.vertices[2]); 
        tri.vertices = {a,b,c};
    }
}

void Mesh::xrotate(float angle){
    for (auto& tri: triangles){
        tri.vertices[0] = maths::mul(maths::x_rotation(angle), tri.vertices[0]);
        tri.vertices[1] = maths::mul(maths::x_rotation(angle), tri.vertices[1]);
        tri.vertices[2] = maths::mul(maths::x_rotation(angle), tri.vertices[2]);
    }
}
void Mesh::yrotate(float angle){
    for (auto& tri: triangles){
        tri.vertices[0] = maths::mul(maths::y_rotation(angle), tri.vertices[0]);
        tri.vertices[1] = maths::mul(maths::y_rotation(angle), tri.vertices[1]);
        tri.vertices[2] = maths::mul(maths::y_rotation(angle), tri.vertices[2]);
    }
}
void Mesh::zrotate(float angle){
    for (auto& tri: triangles){
        tri.vertices[0] = maths::mul(maths::z_rotation(angle), tri.vertices[0]);
        tri.vertices[1] = maths::mul(maths::z_rotation(angle), tri.vertices[1]);
        tri.vertices[2] = maths::mul(maths::z_rotation(angle), tri.vertices[2]);
    }
}

void Mesh::scale(float sx, float sy, float sz){
    for (auto& tri: triangles){
        tri.vertices[0] = maths::mul(maths::scale(sx,sy,sz), tri.vertices[0]);
        tri.vertices[1] = maths::mul(maths::scale(sx,sy,sz), tri.vertices[1]);
        tri.vertices[2] = maths::mul(maths::scale(sx,sy,sz), tri.vertices[2]);
    }
}

void Mesh::setProjection(maths::mat4f proj){
    projection = proj;
}
void Mesh::setView(maths::mat4f vi){
    view= vi;
}
void Mesh::render(){
    
    finalTris.clear();

    for (auto& tri:triangles){
        Triangle temptri = tri;

        // Culling
        if (backFaceCulling(temptri)){
            continue;
        }
        
        //Shading
        temptri.color = {220,200,180};
        if (isGouraudShade)
            gouraudShading(temptri);
        else
            flatShading(temptri);
            
        //View Transform
        temptri.vertices[0] = maths::mul(view, temptri.vertices[0]);
        temptri.vertices[1] = maths::mul(view, temptri.vertices[1]);
        temptri.vertices[2] = maths::mul(view, temptri.vertices[2]);


        // Projection Transformation and Normalization
        temptri.vertices[0] = maths::mul(projection, temptri.vertices[0]);
        temptri.vertices[1] = maths::mul(projection, temptri.vertices[1]);
        temptri.vertices[2] = maths::mul(projection, temptri.vertices[2]);
        
        // Viewport Transformation
        temptri.vertices[0] = maths::mul(maths::translate(1.0,1.0,0.0),temptri.vertices[0]);
        temptri.vertices[1] = maths::mul(maths::translate(1.0,1.0,0.0),temptri.vertices[1]);
        temptri.vertices[2] = maths::mul(maths::translate(1.0,1.0,0.0),temptri.vertices[2]);
        //.continued
        temptri.vertices[0] = maths::mul(maths::scale(0.5*canvas->scrWidth,0.5*canvas->scrHeight,1.0),temptri.vertices[0]);
        temptri.vertices[1] = maths::mul(maths::scale(0.5*canvas->scrWidth,0.5*canvas->scrHeight,1.0),temptri.vertices[1]);
        temptri.vertices[2] = maths::mul(maths::scale(0.5*canvas->scrWidth,0.5*canvas->scrHeight,1.0),temptri.vertices[2]);

        temptri.populateVertices();

        finalTris.push_back(temptri);
    }

        //Depth buffer -- painter's algorithm
    // std::sort(finalTris.begin(), finalTris.end(), [](Triangle &t1, Triangle &t2)
    // {
    //     float z1 = (t1.vertex[0].position[2] + t1.vertex[1].position[2] + t1.vertex[2].position[2]) / 3.0f;
    //     float z2 = (t2.vertex[0].position[2] + t2.vertex[1].position[2] + t2.vertex[2].position[2]) / 3.0f;
    //     return z1 > z2;
    // });


    for (auto& tri:finalTris){
        if (isWireframe)
            tri.wireframe_draw();
        else
            tri.rasterize();
    }
}

bool Mesh::backFaceCulling(Triangle& tri){
    maths::vec3f v1 = tri.vertices[0];
    maths::vec3f v2 = tri.vertices[1];
    maths::vec3f v3 = tri.vertices[2];

    maths::vec3f centroid;
    centroid[0] = (v1[0] + v2[0] + v3[0]) / 3; 
    centroid[1] = (v1[1] + v2[1] + v3[1]) / 3; 
    centroid[2] = (v1[2] + v2[2] + v3[2]) / 3;

    maths::vec3f normal = maths::getnormal(centroid,v2,v3);
    maths::vec3f view = maths::normalize(maths::sub(camera->m_pos,centroid));
    float dotProduct = maths::dot(normal,view);

    if(dotProduct<0){
        return false;
    }
    return true;
}

float Mesh::calculateIntensity(maths::vec3f point, maths::vec3f normal, maths::vec3f view){

    maths::vec3f position = {10,10,10};
    maths::vec3f l_dir = maths::normalize(maths::sub(position,point));
    float ambientInt = 0.9;
    float pointInt = 0.5;

    float ambientConstant = 1;
    float diffuseConstant = 0.7;
    float specularConstant = 0.8;

    float ambientLight = ambientConstant*ambientInt;
    
    float diffuseLight = maths::max(diffuseConstant* 1 *maths::dot(normal,l_dir),0.0f);

    maths::vec3f reflection = maths::normalize(maths::sub(maths::mul(normal,(2* maths::dot(normal,l_dir))),l_dir));
    float specularLight = specularConstant * pointInt * pow(maths::dot(reflection,view),32);
    
    float tmp = ambientLight+diffuseLight;
    tmp = (tmp > 1) ? 1: tmp;
    // tmp = (tmp < 0.8) ? 0.8 : tmp;
    // std::cout << tmp << '\t';
    return tmp;
}


void Mesh::flatShading(Triangle& tri){

    maths::vec3f v1 = tri.vertices[0];
    maths::vec3f v2 = tri.vertices[1];
    maths::vec3f v3 = tri.vertices[2];

    maths::vec3f centroid;
    centroid[0] = (v1[0] + v2[0] + v3[0]) / 3; 
    centroid[1] = (v1[1] + v2[1] + v3[1]) / 3; 
    centroid[2] = (v1[2] + v2[2] + v3[2]) / 3;

    maths::vec3f normal = maths::getnormal(centroid,v2,v3);
    maths::vec3f view = maths::normalize(maths::sub(camera->m_pos,centroid));

    float intensity = calculateIntensity(centroid,normal,view);
    tri.setIntensity(maths::vec3f{intensity,intensity,intensity});
}
    
void Mesh::gouraudShading(Triangle& tri){
    maths::vec3f intensity;
    int count = 0;
    for (auto& vertex: tri.vertices){
        maths::vec3f view = maths::normalize(maths::sub(camera->m_pos,vertex));
        intensity[count] = calculateIntensity(vertex,tri.normals[count],view);
        count++;
    }
    tri.setIntensity(intensity);
}

