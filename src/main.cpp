#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>

using namespace std;

vector<string> split(string Data)
{
    size_t pos = 0;
    string token;
    string delimiter = " ";
    string s = Data;
    vector<string> OutPut;
    while ((pos = s.find(delimiter)) != string::npos)
    {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        OutPut.push_back(token);
    }

    OutPut.push_back(s);

    return OutPut;
}

struct point
{
    float x = 0, y = 0, z = 0;


    // Tester chaque coordonnees, peut etre lent, je sais pas.
    bool operator==(point& p1)
    {
        return this->x == p1.x && this->y == p1.y && this->z == p1.z;
    }

    bool operator!=(point& p1)
    {
        return this->x != p1.x || this->y != p1.y || this->z != p1.z;
    }
};

struct arete
{
    point p1, p2;

    // Assez evident
    bool contains(point p)
    {
        return p1 == p || p2 == p;
    }

    // Evident aussi
    bool operator==(arete& a1)
    {
        return (this->p1 == a1.p1 && this->p2 == a1.p2) || (this->p2 == a1.p1 && this->p1 == a1.p2);
    }

    // Evident
    bool touches(arete a1)
    {
        return a1.contains(this->p1) || a1.contains(this->p2);
    }

    // Evident
    point intersection(arete a)
    {
        if (this->p1 == a.p1) return this->p1;
        if (this->p2 == a.p1) return this->p2;
        if (this->p1 == a.p2) return this->p1;
        if (this->p2 == a.p2) return this->p2;
    }

    // Theoreme de pythagore mais en 3d avec des vecteurs, vu que cest pas forcement rectangle... je crois
    float dist()
    {
        return sqrt(pow(this->p1.x-this->p2.x, 2) + pow(this->p1.y-this->p2.y, 2) + pow(this->p1.z-this->p2.z, 2));
    }
};

struct face
{
    point p1, p2, p3;

    // Tester toute les combinaisons de faces (peut etre inutil je sais pas...)
    bool operator==(face& f1)
    {
        return
            (this->p1 == f1.p1 && this->p2 == f1.p2 && this->p3 == f1.p3)
            ||
            (this->p1 == f1.p1 && this->p2 == f1.p3 && this->p3 == f1.p2)
            ||
            (this->p1 == f1.p2 && this->p2 == f1.p3 && this->p3 == f1.p1)
            ||
            (this->p1 == f1.p2 && this->p2 == f1.p1 && this->p3 == f1.p3)
            ||
            (this->p1 == f1.p3 && this->p2 == f1.p1 && this->p3 == f1.p2)
            ||
            (this->p1 == f1.p3 && this->p2 == f1.p2 && this->p3 == f1.p1)
            ;
    }
};

int indexOf(vector<point> p, int length, point pp)
{
    for(int i = 0; i < length; i++)
    {
        if(p[i] == pp)
        {
            return i;
        }
    }
    return -1;
}

point toCoords(float dist, float lidar, float motor)
{

    point p = {dist * sin(lidar), dist * cos(lidar) * cos(motor), dist * cos(lidar) * sin(motor)};

    return p;
}

vector<point> getCoords(bool hasArgs, char* argv[])
{
    ifstream TheFileI;
    string s = "";
    cout << "Enter input path" << endl;
if(hasArgs)
    {
        cout << argv[2] << endl;
        s = argv[2];
    }
    else
    {
            cin >> s;
    }
    TheFileI.open(s);
    string tp;
    vector<point> output;
    cout << "Collecting data..." << endl;
    while(getline(TheFileI, tp))
    {
        vector<string> ss = split(tp);

        //if(stof(ss[3]) > 10)

        output.push_back(toCoords(stof(ss[0]), stof(ss[1]) * M_PI / 180, stof(ss[2]) * M_PI / 180 ));
    }
    cout << "Done." << endl;

    TheFileI.close();
    return output;
}

vector<point> getCompiledCoords(bool hasArgs, char* argv[])
{
    ifstream TheFileI;
    string s = "";
    cout << "Enter input path" << endl;
if(hasArgs)
    {
        cout << argv[2] << endl;
        s = argv[2];
    }
    else
    {
            cin >> s;
    }
    TheFileI.open(s);
    string tp;
    vector<point> output;
    cout << "Collecting data..." << endl;
    while(getline(TheFileI, tp))
    {
        vector<string> ss = split(tp);
        point p = {stof(ss[0]), stof(ss[1]), stof(ss[2])};
        output.push_back(p);
    }
    cout << "Done." << endl;
    TheFileI.close();
    return output;
}

int main(int argc, char *argv[])
{
    bool hasArgs = argc == 7;
    cout << "Console On." << endl;


    cout << "0 for compiled coords, 1 for angles and distances" << endl;

    int sss = 0;
    vector<point> points;
    if(hasArgs)
    {
        cout << argv[1] << endl;
        sss = stof(argv[1]);
    }
    else
    {
            cin >> sss;
    }
    switch(sss)
    {
    case 0:
        points = getCompiledCoords(hasArgs, argv);
        break;
            case 1:
        points = getCoords(hasArgs, argv);
        break;
    }

        int pts = points.size();


        string output_obj = "";
    cout << "Enter output path" << endl;
    if(hasArgs)
    {
        cout << argv[3] << endl;
        output_obj = argv[3];
    }
    else
    {
    cin >> output_obj;
    }


    // clear file
    ofstream theFile;
    theFile.open (output_obj);
    theFile << endl;
    theFile.close();

    theFile.open (output_obj, ios_base::app);


    // Je sais pas combien il en faut du coup je fais grand.
    arete* aretes = new arete[1000000];
    face* faces = new face[1000000];


    int currArete = 0;
    int currFace = 0;
    float maxi = 5;
    float mini = 0.07;
    cout << "enter min distance" << endl;
    if(hasArgs)
    {
        cout << argv[4] << endl;
        mini = stof(argv[4]);
    }
    else
    {
            cin >> mini;
    }
    cout << "enter max distance" << endl;
    if(hasArgs)
    {
        cout << argv[5] << endl;
        maxi = stof(argv[5]);
    }
    else
    {
            cin >> maxi;
    }

    cout << "Writing vertices" << endl;
    // Deja ecrire les points (v pour vertex)
    for(int i = 0; i < pts; i++)
    {
        theFile << "v " << points[i].x << " " << points[i].y << " " << points[i].z << endl;
    }

    cout << "Detecting edges" << endl;

    // Creer les aretes
    for (int i = 0; i < pts; i++)
    {
        for (int j = 0; j < pts; j++)
        {
            // Une arête est composée de deux points distincts
            if (i == j)
                continue;

            arete a = {points[i], points[j]};
            float dist = a.dist();
            if (dist > maxi || dist <= mini)
                continue;



            bool shouldnt_create = false;
            for (int k = 0; k < currArete; k++)
            {
                if (aretes[k] == a)
                {
                    shouldnt_create = true;
                    break;
                }
            }

            if (!shouldnt_create)
                aretes[currArete++] = a;
        }
    }

    // Algo pour creer les faces


    cout << "Assembling edges..." << endl ;
    int faceCounter = 0;
    int areteLen = currArete;
    cout << areteLen << " edges were detected" << endl;
    int printEach = stoi(argv[6]);
    for (int a = 0; a < areteLen; a++)
    {
        for (int b = 0; b < areteLen; b++)
        {

            if(aretes[a].touches(aretes[b]))
            {

                for (int c = 0; c < areteLen; c++)
                {

                    // seulement montrer le compteur une fois sur 50
                    if(currFace > faceCounter && currFace % printEach == 0)
                    {
                        faceCounter = currFace;
                        cout << "Assembling face number " << currFace << endl;
                    }

                    // Une face est composée de trois arêtes différentes

                    if(a == b || b == c || c == a)
                        continue;

                    arete A = aretes[a];
                    arete B = aretes[b];
                    arete C = aretes[c];


                    point p1 = A.intersection(B);
                    point p2 = B.intersection(C);
                    point p3 = C.intersection(A);

                    if(p1 == p2 || p2 == p3 || p3 == p1)
                        continue;

                    // On teste si les 3 faces forment une ligne fermée

                    if (A.touches(B) && B.touches(C) && C.touches(A))
                    {
                        // Dans ce cas, on ajoute peut-être la face

                        face f = {p1, p2, p3};

                        bool err = false;
                        for(int hh = 0; hh  < currFace; hh++)
                        {
                            if(faces[hh] == f)
                            {
                                err = true;
                                break;
                            }
                        }

                        if(!err)
                        {
                            faces[currFace++] = f;
                        }


                    }
                }
            }
        }
    }

    cout << currFace << " faces detected" << endl;
    cout << "Writing faces..." << endl;

    // Ecrire les faces (f pour face)
    for(int i = 0; i < currFace; i++)
    {
        theFile << "f " << indexOf(points, pts, faces[i].p1)+1 << " " << indexOf(points, pts, faces[i].p2)+1 << " " << indexOf(points, pts, faces[i].p3)+1 << endl;
    }
    cout << "Done." << endl;
    theFile.close();


    delete[] aretes;
    delete[] faces;


    return 0;
}
