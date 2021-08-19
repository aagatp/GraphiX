#include "mesh.h"
#include <algorithm>

Mesh::Mesh(Canvas* mcanvas, Light* mlight){
    canvas = mcanvas;
    light = mlight;
    projection = maths::matidentity();
    view = maths::matidentity();
    isWireframe = true;
    isGouraudShade = false;
    isFlatShade = true;
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
    
    int count =0;
    while (!in.eof())
    {
        //get one line at a time
        std::getline(in, line);
        //string object
        std::istringstream iss(line.c_str());

        char trash;
        if (!line.compare(0, 2, "v "))  //starts with v<space>
        {   
            iss >> trash; // first character is v
            maths::vec3f v;
            iss >> v[0];
            iss >> v[1];
            iss >> v[2];
            verts.push_back(v);
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
        else if (!line.compare(0,2,"o ")){
            count++;
        }
        else if (!line.compare(0, 2, "f ")) //starts with f<space>
        {
            std::vector<maths::vec3i> f;
            maths::vec3i temp;

            iss >> trash; //first charecter is f

            while (iss >> temp[0] >> trash >> temp[1] >> trash >> temp[2])
            {
                //in wavefront obj all indices start at 1, not zero
                temp[0]--;   //vert
                temp[1]--;   //texture
                temp[2]--; // normal 
                f.push_back(temp);
            }
            Triangle tri(canvas);
            tri.setVertex(verts[f[0][0]],verts[f[1][0]],verts[f[2][0]]);
            tri.setTexCoords(textures[f[0][1]],textures[f[1][1]],textures[f[2][1]]);
            tri.setNormals(normals[f[0][2]],normals[f[1][2]],normals[f[2][2]]);

            if (count >14 && count < 27)
                tri.setColor(colors[0]);
            if (count==14)
                tri.setColor(colors[3]);
            if (count > 27 && count <38)
                tri.setColor(colors[1]);
            if (count <= 13)
                tri.setColor(colors[2]);
            if (count==39)
                tri.setColor(colors[7]);
            if (count >39 && count<45)
                tri.setColor(colors[5]);
            if (count>=45)
                tri.setColor(colors[4]);
            
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

void Mesh::processKeyboard(char key, float dt){
    switch (key){
        case 'e':
            isWireframe = !isWireframe;
            break;
        case 'g':
            isGouraudShade = true;
            isFlatShade = false;
            break;
        case 't':
            isFlatShade = true;
            isGouraudShade = false;
            break;
        case 'j':
            xrotate(0.05);
            break;
        case 'k':
            xrotate(-0.05);
            break;
        case 'h':
            yrotate(0.05);
            break;
        case 'l':
            yrotate(-0.05);
            break;
    }
}

void Mesh::render(){
    
    finalTris.clear();
    int count = 0;
    for (auto tri:triangles){
        Triangle temptri = tri;
        
        // Culling
        if (backFaceCulling(temptri)){
            continue;
        }
        if (isFlatShade)
            flatShading(temptri);
        else if (isGouraudShade)
            gouraudShading(temptri);
        
        // isGouraudShade = false;
        //View Transform
        temptri.vertices[0] = maths::mul(view, temptri.vertices[0]);
        temptri.vertices[1] = maths::mul(view, temptri.vertices[1]);
        temptri.vertices[2] = maths::mul(view, temptri.vertices[2]);

        // Projection Transformation and Normalization
        temptri.vertices[0] = maths::mul(projection, temptri.vertices[0]);
        temptri.vertices[1] = maths::mul(projection, temptri.vertices[1]);
        temptri.vertices[2] = maths::mul(projection, temptri.vertices[2]);
        
        // Viewport Transformation

        maths::mat4f viewport = maths::mul(maths::translate(1.0,1.0,0.0),maths::scale(0.5*canvas->scrWidth,0.5*canvas->scrHeight,1.0));
        
        temptri.vertices[0] = maths::mul(maths::translate(1.0,1.0,0.0),temptri.vertices[0]);
        temptri.vertices[1] = maths::mul(maths::translate(1.0,1.0,0.0),temptri.vertices[1]);
        temptri.vertices[2] = maths::mul(maths::translate(1.0,1.0,0.0),temptri.vertices[2]);
        
        temptri.vertices[0] = maths::mul(maths::scale(0.5*canvas->scrWidth,0.5*canvas->scrHeight,1.0),temptri.vertices[0]);
        temptri.vertices[1] = maths::mul(maths::scale(0.5*canvas->scrWidth,0.5*canvas->scrHeight,1.0),temptri.vertices[1]);
        temptri.vertices[2] = maths::mul(maths::scale(0.5*canvas->scrWidth,0.5*canvas->scrHeight,1.0),temptri.vertices[2]);
        
        finalTris.push_back(temptri);

        // //Shadows
        // shadowtri.vertices[0] = maths::mul(light->getLightTransform(), temptri.vertices[0]);
        // shadowtri.vertices[1] = maths::mul(light->getLightTransform(), temptri.vertices[1]);
        // shadowtri.vertices[2] = maths::mul(light->getLightTransform(), temptri.vertices[2]);

        // shadowtri.vertices[0] = maths::mul(maths::translate(1.0,1.0,0.0),shadowtri.vertices[0]);
        // shadowtri.vertices[1] = maths::mul(maths::translate(1.0,1.0,0.0),shadowtri.vertices[1]);
        // shadowtri.vertices[2] = maths::mul(maths::translate(1.0,1.0,0.0),shadowtri.vertices[2]);
        
        // shadowtri.vertices[0] = maths::mul(maths::scale(0.5*canvas->scrWidth,0.5*canvas->scrHeight,1.0),shadowtri.vertices[0]);
        // shadowtri.vertices[1] = maths::mul(maths::scale(0.5*canvas->scrWidth,0.5*canvas->scrHeight,1.0),shadowtri.vertices[1]);
        // shadowtri.vertices[2] = maths::mul(maths::scale(0.5*canvas->scrWidth,0.5*canvas->scrHeight,1.0),shadowtri.vertices[2]);

        // shadowTris.push_back(shadowtri);
    }
    for (auto tri:finalTris){
        if (isWireframe)
            tri.wireframe_draw();
        else{
            tri.rasterize();
        }
    }

    // for (auto tri:shadowTris){
    //     tri.shadowRasterize();
    // }
    
}

bool Mesh::backFaceCulling(Triangle& tri){

    maths::vec3f v1 = tri.vertices[0];
    maths::vec3f v2 = tri.vertices[1];
    maths::vec3f v3 = tri.vertices[2];

    maths::vec3f centroid = maths::centroid(v1,v2,v3);
    maths::vec3f normal = maths::getnormal(centroid,v2,v3);
    maths::vec3f view = maths::normalize(maths::sub(centroid,camera->m_pos));

    float dotProduct = maths::dot(normal,view);
    return dotProduct < 0 ? false : true;
}


void Mesh::flatShading(Triangle& tri){

    maths::vec3f v1 = tri.vertices[0];
    maths::vec3f v2 = tri.vertices[1];
    maths::vec3f v3 = tri.vertices[2];

    maths::vec3f centroid = maths::centroid(v1,v2,v3);
    maths::vec3f normal = maths::getnormal(centroid,v2,v3);
    maths::vec3f view = maths::normalize(maths::sub(camera->m_pos,centroid));

    float intensity = light->calculateIntensity(centroid,normal,view);
    tri.setIntensity(maths::vec3f{intensity,intensity,intensity});
}
    
void Mesh::gouraudShading(Triangle& tri){

    maths::vec3f intensity;
    int count = 0;
    for (auto& vertex: tri.vertices){
        maths::vec3f view = maths::normalize(maths::sub(camera->m_pos,vertex));
        intensity[count] = light->calculateIntensity(vertex,tri.normals[count],view);
        count++;
    }
    tri.setIntensity(intensity);
}