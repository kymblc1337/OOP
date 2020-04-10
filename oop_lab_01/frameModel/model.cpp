#include "model.h"
#include "GLUT/glut.h"

int modelInitFromFile(myModel& model)
{
    myModel tmp;
    modelBasicInit(tmp);

    FILE *f;
    f = fopen(myFilename, "r");

    if (modelfillPoints(f, tmp) != OK)
    {
        return ERRORWHILEREADINGPOINTS;
    }
    if (modelfillEdges(f, tmp) != OK)
    {
        return ERRORWHILEREADINGEDGES;
    }
    modelGetCenter(tmp);
    modelReCalculatePoints(tmp);
    tmp.isInited = true;
    freeMyMemory(model);
    modelCopy(tmp, model);
    freeMyMemory(tmp);
    return OK;
}

int modelfillPoints(FILE* f, myModel& model)
{
    int n;
    int ret = NOFILE;
    if (f != NULL)
    {
        ret = OK;
        if (fscanf(f, "%d", &n) != 1)
        {
            return NOTENOUGHPOINTS;
        }

        model.num_of_points = n;
        model.masOfPointsOffset = new myPoint[n];
        if (model.masOfPointsOffset == NULL)
        {
            return NOFREESPACE;
        }
        if (createMatrix(model.edges, model.num_of_points) != OK)
        {
            return NOFREESPACE;
        }

        for (int i = 0; i < n; i++)
        {
            double x, y, z;
            if (fscanf(f, "%lf %lf %lf", &x, &y, &z) != 3)
            {
                ret = NOTENOUGHPOINTS;
            }
            initPoint(model.masOfPointsOffset[i], x, y, z);
        }
    }
    return ret;
}


int modelfillEdges(FILE* f, myModel& model)
{
    int n;
    int ret = NOFILE;
    if (f != NULL)
    {
        ret = OK;
        if (fscanf(f, "%d", &n) != 1)
        {
            ret = NOTENOUGHEDGES;
        }
        for (int i = 0; i < n; i++)
        {
            int a, b;
            if (fscanf(f, "%d %d", &a, &b) != 2)
            {
                ret = NOTENOUGHEDGES;
            }
            matrixSetElement(model.edges, 1, a, b);
            matrixSetElement(model.edges, 1, b, a);
        }
    }
    fclose(f);
    return ret;
}


void modelGetCenter(myModel& model)
{
    double xmin = model.masOfPointsOffset[0].x, xmax = model.masOfPointsOffset[0].x;
    double ymin = model.masOfPointsOffset[0].y, ymax = model.masOfPointsOffset[0].y;
    double zmin = model.masOfPointsOffset[0].z, zmax = model.masOfPointsOffset[0].z;
    for(int i = 0; i < model.num_of_points; i++)
    {
        if(model.masOfPointsOffset[i].x > xmax) {
            xmax = getPointX(model.masOfPointsOffset[i]);
        } else if(model.masOfPointsOffset[i].x < xmin) {
            xmin = getPointX(model.masOfPointsOffset[i]);
        }

        if(model.masOfPointsOffset[i].y > ymax) {
            ymax = getPointY(model.masOfPointsOffset[i]);;
        } else if(model.masOfPointsOffset[i].x < ymin) {
            ymin = getPointY(model.masOfPointsOffset[i]);;
        }

        if(model.masOfPointsOffset[i].z > zmax) {
            zmax = getPointZ(model.masOfPointsOffset[i]);
        } else if(model.masOfPointsOffset[i].z < zmin) {
            zmin = getPointZ(model.masOfPointsOffset[i]);
        }
    }
    initPoint(model.center, ((xmax + xmin) / 2), ((zmax + zmin) / 2), ((zmax + zmin) / 2));
}

void modelReCalculatePoints(myModel& model)
{
    for(int i = 0; i < model.num_of_points; i++)
    {
        double newx = getPointX(model.masOfPointsOffset[i]) - getPointX(model.center);
        double newy = getPointY(model.masOfPointsOffset[i]) - getPointY(model.center);
        double newz = getPointZ(model.masOfPointsOffset[i]) - getPointZ(model.center);
        initPoint(model.masOfPointsOffset[i], newx, newy, newz);
    }
}

void modelMoveCenter(myModel& model, double dx, double dy, double dz)
{
    pointMove(model.center, dx, dy, dz);
}

void freeMyMemory(myModel& model)
{
    delete model.masOfPointsOffset;
    freeMyMatrix(model.edges);
    model.num_of_points = 0;
    model.isInited = false;
}

void modelCopy(myModel& source, myModel& dest)
{
    modelBasicInit(dest);
    dest.isInited = source.isInited;
    dest.num_of_points = source.num_of_points;
    dest.center = source.center;
    dest.masOfPointsOffset = new myPoint[dest.num_of_points];
    for(int i = 0; i < dest.num_of_points; i++)
    {
        dest.masOfPointsOffset[i] = source.masOfPointsOffset[i];
    }
    copyMatrix(source.edges, dest.edges);
}

void modelBasicInit(myModel &model)
{
    model.num_of_points = 0;
    model.isInited = false;
    model.masOfPointsOffset = NULL;
    createEmptyMatrix(model.edges);
}