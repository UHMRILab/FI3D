# FI3D Messaging Protocol - Module Messages

Module messages are the most convoluted type of messages because there are many different types of module messages. Module messages are used by the FIs to control and manipulate the module. In order for an FI to control the module, it first has to subscribe to the module instance it intends to interface with.

A subscription message notifies the system that the FI wants to stay updated with all pertinent information about  the subscribed module. Once subscribed, the subscribed module will continously send messages to the FI with every change to the visuals and module interactions. If the FI wishes to not receive these updates, they can unsubscribe from the module. An FI that is not subscribed to a Module cannot interact with the module. There are two ways that FIs interact with the module, by modifying module interactions and its visuals. Thus, all the Request/Responses revolve around these two.

For more information on how the module interactions and visuals work, see the [Module Architecture]() document. In essence, module interactions are simple values that can be given to the module (think of it as module parameters). Visuals are the different renderings on the screen.

### Module Interactions

The following module interactions are available:

| Interaction Type  | DataType  | Value | Description |
| ---               | ---       | ---   | --- |
| Valueless         | None      |  1    | Triggers an action, does not need a value |
| Bool              | bool      |  2    | Used to enable/disable an action |
| Integer           | int       |  3    | Provides a whole integer numeric value |
| Float             | float     |  4    | Provides a floating numeric value |
| String            | string    |  5    | Provides a string value |
| Select            | int       |  6    | Provides an index corresponding to a selection among a set of values

<br>Interactions may have constraints. For example, an integer interaction may have a maximum allowed number that can be entered. Therefore, the following constraints are available for the corresponding interaction.

| Constraint Type   | Applies to        | Value | Description |
| ---               | ---               | ---   | --- |
| None              | All               |  0    | No constraint |
| Min               | int, float        |  1    | The value may not be less than this |
| Max               | int, float        |  2    | The value may not be greater than this |
| Range             | int, Float        |  3    | The value may not be less or grater than these |
<br>

### Visuals

There are four different types of visuals, which are identified as follows:

| Visual Type | Value | Description |
| --- | --- | --- |
| ImageSlice        | 1 | A 2D sub-array of a 3D-Image (AKA, slice) |
| StudySlice        | 2 | Same as ImageSlice, but belongs to a Study |
| Model             | 3 | The polygonal mesh |
| Assembly          | 4 | Various Models stitched together as a single Model |

<br>Visuals have a transformation matrix that defines their position, scale, and orientation in 3D space. They can also have a parent Visual. When a parent Visual is there, the transformation matrix is with respect to the Parent's origin. When there is no parent, the transformation is with respect to the origin of the 3D space. All visuals also have opacity and visibiliy. All Images (both ImageSlice and StudySlice) have an orientation, indicating the 2D subarray index within the 3D Image. The orientation of the subarray is identified by the following:

| Slice Orientation | Value |
| --- | --- |
| XY (Transverse)   | 1 |
| YZ (Sagittal)     | 2 |
| XZ (Coronal)      | 3 |

<br>

## Responses

Module responses are automatically sent to subscribed FIs. These responses are produced due to changes within the module. The following table describes the type of responses the FI3D can send to FIs.

| Module Response ID | Value | Description |
| --- | --- | --- |
| AddModuleInteraction      |  1 | Adds the module interaction |
| UpdateModuleInteraction   |  2 | Updates the value and (if needed) constraint of the Module Interaction |
| RemoveModuleInteraction   |  3 | Removes the module interaction |
| AddVisual                 |  4 | Add a new visual to the scene |
| RefreshVisual             |  5 | Refreh the visual, send all its visual information |
| RemoveVisual              |  6 | Removes a visual from the scene |
| DataChange                |  7 | Informs that the data being rendered by the visual has changed |
| HideVisual                |  8 | Visual's visibility has change |
| TransformVisual           |  9 | Visual has moved (i.e, rotated, translated, or scaled) |
| ParentChange              | 10 | The parent of the visual has changed |
| SetVisualOpacity          | 11 | Visual's opacity has changed |
| SetSlice                  | 12 | ImageSlice index has changed, for Images only |
| SetColor                  | 13 | Model's color has change, for Models only |

<br>Note that keys that are not relevat to the ResponseID are not sent to reduce packet size. For example, a "SetColor" `ResponseID` does not include whether the Visual is hidden or not. The following keys are additional to Module messages.

| Key | Value Type | Description |
| --- | --- | --- |
| ModuleInfo | JsonObject | Object containig all the module response information |
| ModuleID | string | The ID of the module this response pertains to |
| SceneID  | string | The ID of the scene of the module |
| ModuleInteractions | JsonArray | Array containing Module Interactions information |
| VisualsInfo  | JsonArray | Array containing Visuals information |

<br>The `ModuleInteractions` is an array of JsonObjects containing information for each module interaction. This object has the following keys:

| Key | Value Type | Description |
| --- | --- | --- |
| ResponseID | int | The type of module response, can only be 1, 2 or 3 (see Module Response ID Table)  |
| ID | string | The ID of the module interaction |
| Type  | int | The type of module interaction |
| Value | JsonValue | The value of the module interaction, can be any data type depending on the interaction |
| ValueOptions  | string array | Contains the options for `Type` = `Select` |
| Constraint  | int | The constraint of the module interaction |
| UpdateConstraint  | JsonArray | If `ResponseID` = 2, whether the constraint should be updated as well |
| MinValue  | int or float | The minimum value allowed for `Constraint` = `Min` or `Range` |
| MaxValue  | int or float | The maximum value allowed for `Constraint` = `Max` or `Range` |
| Info  | string | Description of what the module intraction does |

<br> The `VisualsInfo` is an array of JsonObjects containing information for each visual. This object has the following keys:

| Key | Value Type | Description |
| --- | --- | --- |
| ResponseID | int | The type of module response, can only be 4 to 13 (see Module Response ID Table)  |
| ID | string | The ID of the visual |
| Type  | int | The type of visual |
| Visible | bool | Whether the visual is visible or invisible |
| Opacity  | float | The opacity of the visual: from 0 to 1 |
| Transformation  | Double[16] | 4x4 matrix representing the visual's orientation, position, and scale |
| ParentID  | string | The ID of the visual parent |
| DataID  | string | The ID of the data being rendered by the visual. Does not Apply to `Assembly` |
| DataName  | string | The name of the data being rendered by the visual. Does not Apply to `Assembly` |
| DataType  | int | The data type being rendered by the visual. Does not Apply to `Assembly` |
| Color  | double[3] | The color, in RGB. Only applies to `Model` |
| SliceIndex  | int | The index of the slice. Only applies to `ImageSlice` and `StudySlice` |
| SliceOrientation  | int | The orientation of the slice. Only applies to `ImageSlice` and `StudySlice` |
| SeriesIndex  | int | The index of the series. Only applies to `StudySlice` |
| AssemblyPartCount  | int | Number of parts in an Assembly. Only applies to `Assembly` |
| AssemblyPartsInfo  | JsonArray | Array containing information about the Assembly parts. Only applies to `Assembly`. The objects in the array contain the same keys that pertain to `Model` visuals |

<br>Assembly visuals have no data. Instead, they represent a group of Model visuals as one. The data making up the Assembly are contained within its parts (Models that make up the Assembly). The assembly parts are not part of the scene, the assembly is. That means that parenting of parts only works when the parent is also part of the assembly. The visual IDs of the parts cannot be the same, but they could be the same to a Visual outside of the assembly. Because Assembly visuals have no data, the `ResponseID` works as follows:

- On AddVisual, it includes the assembly's information and its parts' information.
- On DataChange, there was a change to a part in the assembly. The `ResponseID` within the part info determines what the change is. Only the affected parts are sent.
  - AddVisual - The VisualInfo received is to add that part to the assembly
  - DataChange - The rendered model of that part has changed.
  - RefreshVisual - There was a change to the part (transformation, opacity change, etc.)
  - The rest of the AssemblyActions do as expected. Only Model related actions are used (e.g., There is no SetSlice in `ResponseID` for an Assembly part).
- On RefreshVisual, there was a change to the assembly (not its parts). No parts' information is sent.


### Examples

When first subscribed to a module, the module sends a message with every single module interaction and visual information. The message is similar to the following:

``` JavaScript
{
    ResponseStatus: 1,
    MessageType: 3,
    ModuleInfo: {
        ModuleID: "Cardiac - 1",
        SceneID: "Cardiac Scene",
        ModuleInteractions: [       
            // Example of Integer interaction with Range constraint                
            {
                ResponseID: 1,
                ID: "Cardiac Phase",
                Type: 3,
                Value: 5,
                Constraint: 3,
                UpdateConstraint: false,
                MinValue: 0,
                MaxValue: 13,
                // Notice there is no "ValueOptions" because it is not a "Select" interaction
                Info: "Changes the cardiac phase"
            },
            ... // As many interactions as there are
        ],
        VisualsInfo: [
            // Example of Model
            {           
                ResponseID: 4,
                ID: "Heart",
                Type: 3,
                Visible: true,
                Opacity: 1.0,
                Transformation: [
                    32.21, -0.321, 1.55, -3.0,
                    -89.21, 312.2, 32, 2.1,
                    -3.211, -.778, 0.802, 1.380,
                    8.213, -99.83, -321, -78.2
                ],
                ParentID: "",
                DataID: "{00000000-0000-0000-0000-000000000000}",
                DataName: "Heart Data",
                DataType: 3,
                Color: [0.32, 0.89, 0.15]
            },
            // Example of StudySlice
            {                                       
                ResponseID: 4,
                ID: "Heart Image",
                Type: 2,
                Visible: true,
                Opacity: 1.0,
                Transformation: [
                    43.21, 0.311, 1.55, -3.0,
                    -89.31, 32.2, 32, 7.1,
                    -3.211, -.778, 8.802, 1.380,
                    8.213, -9.83, -321, -7.9
                ],
                ParentID: "",
                DataID: "{00000000-0000-0000-0000-000000000000}",
                DataName: "CINE MRI",
                DataType: 2,
                SliceIndex: 5,
                SliceOrientation: 1,
                SeriesIndex: 8
            },
            ... // As many as there is
        ]
    },
    Message: ""
}
```

For `ResponseID` = `AddModuleInteraction`, if the `Constraint` = `None`, the constraint keys won't be present. The keys in `UpdateModuleInteraction` are the same as `AddModuleInteraction`. For `ResponseID` = `RemoveModuleInteraction`, only the `ID` is given.

Example of `RemoveModuleInteraction`:
``` JavaScript
{
    ResponseStatus: 1,
    MessageType: 3,
    ModuleInfo: {
        ModuleID: "Cardiac - 1",
        SceneID: "Cardiac Scene",
        ModuleInteractions: [       
            {
                ResponseID: 3,
                ID: "Cardiac Phase",
            }
        ],
        VisualsInfo: []
    },
    Message: ""
}
```

For `ResponseID` = `RefreshVisual`, all the same keys as `AddVisual` are present.

## Requests

The following requests can be made to the FI3D server.

| Module Request ID | Value | Description |
| --- | --- | --- |
| ModuleInteraction     |  1 | Changes the value of a module interaction |
| SubscribeToModule     |  2 | Subscribes to the module |
| UnsubscribeToModule   |  3 | Unsubscribes from the module |
| GetScene              |  4 | Asks for information about all the visuals. |
| GetVisual             |  5 | Asks for information about a particular visual. |
| HideVisual            |  6 | Change the visual's visibility |
| TranslateVisual       |  7 | Translate visual by x, y, z |
| RotateVisual          |  8 | Rotate visual by x, y, z (in degrees) |
| SelectSlice           |  9 | Selects the slice by providing slice index, for ImageSlice and StudySlice only |
| SelectOrientation     | 10 | Selects the orientation of the Image, for Imageslice and StudySlice only |
| SelectSeries          | 11 | Selects the series in the study, for StudySlice only |

`ResponseID`'s 2-5 , only affect the FI making the request. The other requests result in updates sent to all subscribed FIs. The following keys can appear in a Module request.

| Key | Value Type | Description |
| --- | --- | --- |
| ModuleParams | JsonObject | Contains information about the module request  |
| ModuleID | string | The ID of the module this request pertains to (the subscribed module) |
| SceneID  | int | The ID of the Scene |
| RequestID | JsonValue | The type of request (see table above) |
| InteractionID | string | The ID of the module interaction, for `ResponseID` = 1-3 |
| InteractionValue | JsonValue | The value of the module interaction, for `ResponseID` = 1-3 |
| TargetVisual  | string | The ID of the visual, for `ResponseID` = 5-11 |
| Visible  | bool | To hide or make visible the visual, for `ResponseID` = 6 |
| Translate  | double[3] | Translates the visual by x, y, z, for `ResponseID` = 7 |
| Rotate  | double[3] | Rotates the visual by x, y, z, for `ResponseID` = 8 |
| SliceIndex  | int | Selects the slice of the Image, for `ResponseID` = 9 |
| SliceOrientation  | int | Selects the orientation of the image, for `ResponseID` = 9 & 10 |
| SeriesIndex | int | Selects the series of the image, for `ResponseID` = 9 - 11 |

<br>Only  the keys relevant to the request need to be sent. The following are examples of the different types of requests.

Example of `ModuleInteraction` request for an Integer module interaction:
``` JavaScript
{  
    ClientID: "FI-1",
    MessageType: 3,
    ModuleParams : {
        ModuleID: "Cardiac",
        SceneID: "Cardiac Scene",
        RequestID: 1,
        InteractionID: "Cardiac Phase",
        InteractionValue: 7,
    },
    Message: ""
}
```

Example of `SubscribeToModule` request:
``` JavaScript
{  
    ClientID: "FI-1",
    MessageType: 3,
    ModuleParams : {
        ModuleID: "Cardiac",
        SceneID: "Cardiac Scene",
        RequestID: 2,
    },
    Message: ""
}
```

Example of `UnsubscribeFromModule` request:
``` JavaScript
{  
    ClientID: "FI-1",
    MessageType: 3,
    ModuleParams : {
        ModuleID: "Cardiac",
        SceneID: "Cardiac Scene",
        RequestID: 3,
    },
    Message: ""
}
```

Example of `GetScene` request:
``` JavaScript
{  
    ClientID: "FI-1",
    MessageType: 3,
    ModuleParams : {
        ModuleID: "Cardiac",
        SceneID: "Cardiac Scene",
        RequestID: 4,
    },
    Message: ""
}
```

Examlpe of `GetVisual` request:
``` JavaScript
{  
    ClientID: "FI-1",
    MessageType: 3,
    ModuleParams : {
        ModuleID: "Cardiac",
        SceneID: "Cardiac Scene",
        RequestID: 5,
        TargetVisual: "Heart",
    },
    Message: ""
}
```

Examlpe of `HideVisual` request:
``` JavaScript
{  
    ClientID: "FI-1",
    MessageType: 3,
    ModuleParams : {
        ModuleID: "Cardiac",
        SceneID: "Cardiac Scene",
        RequestID: 6,
        TargetVisual: "Heart",
        Visible: true,
    },
    Message: ""
}
```

Example of `TranslateVisual` request:
``` JavaScript
{  
    ClientID: "FI-1",
    MessageType: 3,
    ModuleParams : {
        ModuleID: "Cardiac",
        SceneID: "Cardiac Scene",
        RequestID: 7,
        TargetVisual: "Heart",
        Translate: [1, -5, 3.2],
    },
    Message: ""
}
```

Example of `RotateVisual` request:
``` JavaScript
{  
    ClientID: "FI-1",
    MessageType: 3,
    ModuleParams : {
        ModuleID: "Cardiac",
        SceneID: "Cardiac Scene",
        RequestID: 8,
        TargetVisual: "Cardiac",
        Rotate: [-34, 85.3, 138.0],
    },
    Message: ""
}
```

Example of `SelectSlice` request, with change of Orientation too. If the orientation is the same, the key can be ommitted.
``` JavaScript
{  
    ClientID: "FI-1",
    MessageType: 3,
    ModuleParams : {
        ModuleID: "Cardiac",
        SceneID: "Cardiac Scene",
        RequestID: 9,
        TargetVisual: "CINE Image",
        SliceIndex: 9,
        SliceOrientation: 2,
    },
    Message: ""
}
```

Example of `SelectOrientation` request:
``` JavaScript
{  
    ClientID: "FI-1",
    MessageType: 3,
    ModuleParams : {
        ModuleID: "Cardiac",
        SceneID: "Cardiac Scene",
        RequestID: 10,
        TargetVisual: "CINE Image",
        SliceOrientation: 3,
    },
    Message: ""
}
```

Example of `SelectSeries` request:
``` JavaScript
{  
    ClientID: "FI-1",
    MessageType: 3,
    ModuleParams : {
        ModuleID: "Cardiac",
        SceneID: "Cardiac Scene",
        RequestID: 11,
        TargetVisual: "CINE Image",
        SeriesIndex: 20,
    },
    Message: ""
}
```