# FI3D Messaging Protocol - Data Messages

Data messages are used to request data from the system. There are two types of data currently supported: Model and Image. Every data object message contains a flag that states whether that data object can be cacheable by the receiver. When it is false, it indicates the data changes often and it would be better off to not cache it.

### Model

Model data is comprised of polygonal meshes. The mesh is described by an array of points and an array of indices that make up the polygons as follows:

```
Points: [x1, y1, z1, x2, y2, z2, ...]
Line Indices [l1_p1, l1_p2, l2_p1, l2_p2, ...]
Triangle Indices [t1_p0, t1_p1, t1_p2, t2_p0, t2_p1, t2_p2, ...]
```
In terms of triangles, each triangle is constructed by 3 points (hence p0, p1, and p2). The point coordinates are in the `Points` array (hence x1, y1, z1). This works the same for lines. In the future, other polygons may be supported. 

Model data is requested by giving the `DataID` of the Model data. Model data only has one supported format and is identified as follows.

| Model Data Format | Value |
| --- | --- |
| XYZ       | 1 |

<br>

### Image

An image is a 2D array of values. An image can either be one value per array cell (grayscale) or three values per array cell (RGB). Each value represents the intensity in the range 0 to 1. The entire 2D array is sent over as a 1D array, rows next to each other. For example, an image that looks like:

```
v1 v2 v3
v4 v5 v6
v7 v8 v9
```

Is sent over like:
```
[v1, v2, v3, v4, v5, v6, v7, v8, v9]
```

If the image is RGB, then it is sent like:
```
[r1, g1, b1, r2, g2, b2, ...]
```

All images (2D arrays) are part of a 3D array in the FI3D. That is, the image is a 2D subarray of the 3D array. Therefore, an Image can be requested by giving its `DataID`, `Slice Index`, and `Orientation` (XY, YZ, XZ). There are images that are part of a Study. A Study is a collection of 3D-Images, where each 3D-Image of the Study is called a Series. As a result, when an Image is part of a Study, the `SeriesIndex` has to be provided as part of the request. This is because all Series in a Study contain the same `DataID`. The type of data requested is identified as follows:

| Data Type| Value |
| --- | --- |
| Unknown       | 0 |
| Image         | 1 |
| Study         | 2 |
| Model         | 3 |

<br>

Since all Images are part of a 3D-Image, the orientation of the Image (AKA, slice), has to be provided. The orientation is identified as follows:

| Slice Orientation | Value |
| --- | --- |
| XY (Transverse)   | 1 |
| YZ (Sagittal)     | 2 |
| XZ (Coronal)      | 3 |

<br>The format of the data (currently applies to Images only in grascale vs RGB) are identified as follows:

| Image Data Format | Value | Description |
| --- | --- | --- |
| Unknown   | 0 | Considered as an erroneous request |
| Grayscale | 1 | Black and white values |
| RGB       | 2 | Three values indicating color |
| RGBI      | 3 | Like RGB but specifies intensity as well (currently not supported) |

<br>

## Responses

All Data responses are the result of a request. FIs only receive responses to data requests they have made. Data responses can have the additional keys. The Data Type column in the following table indicates which keys are present for all data, Model, Image, or Study. All keys present in Image are also present in Study.

| Key | Data Type | Value Type | Description |
| --- | --- | --- | --- |
| Data          | All | JsonObject | Contains the information of the requested data |
| DataID        | All | string | The ID of the data object |
| DataName      | All | string | The name of the data object, for a Study, this is the same as Study ID. |
| DataType      | All | int | Whether it is a Model, Image, or Study. See Data Type Table above. |
| DataFormat    | All | string | The format of the data (in Image: Grayscale vs RGB). See Image data and Model data format tables |
| Points        | Model | double array | The coordinate information of the points |
| LineIndices   | Model | int array | The indices that make up the line mesh |
| TriangleIndices | Model | int array | The indices that make up the triangle mesh |
| SliceIndex | Image | int | The index of the slice in the 3D-Image data object |
| SliceOrientation | Image | int | The orientation of the slice in the 3D-Image data object |
| SeriesIndex | Study | int | The index of the Series in the Study |
| SeriesCount | Study | int | The count of series within the Study |
| Dimensions | Image | int[3] | Three values representing the dimensions of the 3D-Image |
| Spacing    | Image | double[3] | Three values representing the spacing (width, length, and thickness) of each pixel in the Image |
| Values | Image | Double array | The values the make up the image |

<br>

Here are some examples of data responses for Image, Study, and Model

Grayscale Image:
``` JavaScript
{
    ResponseStatus: 1,
    MessageType: 4,
    Data: {
        DataID: "{00000000-0000-0000-0000-000000000000}",
        DataName: "Image Name",
        DataType: 1,
        Cacheable: true,
        DataFormat: 1,
        SliceIndex: 10,
        SliceOrientation: 1,
        Dimensions: [20, 10, 50],
        Spacing: [0.23, 1.32, 0.88],
        Values: [0.38, 0.09823, ..., 0.088]
    }
    Message: ""
}
```

Grasycale Image of a Study:
``` JavaScript
{
    ResponseStatus: 1,
    MessageType: 4,
    Data: {
        DataID: "{00000000-0000-0000-0000-000000000000}",
        DataName: "Study ID",
        DataType: 2,
        Cacheable: true,
        DataFormat: 1,
        SliceIndex: 10,
        SliceOrientation: 1,
        SeriesIndex: 3,
        SeriesCount: 10,
        Dimensions: [20, 10, 50],
        Spacing: [0.23, 1.32, 0.88],
        Values: [0.38, 0.09823, ..., 0.088]
    }
    Message: ""
}
```

Model:
``` JavaScript
{
    ResponseStatus: 1,
    MessageType: 4,
    Data: {
        DataID: "{00000000-0000-0000-0000-000000000000}",
        DataName: "Model Name",
        DataType: 3,
        Cacheable: true,
        DataFormat: 1,
        Points: [3.280, 1233.082, ...., -13.280],
        LineIndices: [1, 4, ..., 421],
        TriangleIndices: [6, 4, ..., 1]
    }
    Message: ""
}
```

## Requests

A data request uses the similar keys as in data responses. Here is an example for requesting the different data types:

Example of Model
``` JavaScript
{
    ClientID: string,
    MessageTYpe: 4,
    DataParams: {
        DataID: "{00000000-0000-0000-0000-000000000000}",         
        DataType: 3,
    },
    Message: string
}
```

Example of ImageSlice
``` JavaScript
{
    ClientID: string,
    MessageTYpe: 4,
    DataParams: {
        DataID: "{00000000-0000-0000-0000-000000000000}",             
        DataType: 1,
        SliceIndex: 34,
        SliceOrientation: 2,
    },
    Message: string
}
```

Example of StudySlice
``` JavaScript
{
    ClientID: string,
    MessageTYpe: 4,
    DataParams: {
        DataID: "{00000000-0000-0000-0000-000000000000}",             
        DataType: 2,
        SliceIndex: 32,
        SliceOrientation: 1,
        SeriesIndex: 8,
    },
    Message: string
}
```