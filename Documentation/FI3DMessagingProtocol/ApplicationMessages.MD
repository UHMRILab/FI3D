# FI3D Messaging Protocol - Application Messages

Application messages are handled by the FI3D controller object. These type of messages are used to activate and deactivate modules. When a module is activated, the FI3D will create an instance of the module. The FI can also deactivate a module, which asks the FI3D to shut down the module instance. 

## Responses

An Application response is first sent when the user is successfully authenticated (automatically). Application responses include the available modules and the active module instances. Every time a module instance is instantiated or deleted, the FI3D sends an Application response to all authenticated FIs. Application responses have the following additional  keys:

| Key | Value Type | Description |
| --- | --- | --- |
| AvailableModules | JsonArray | Array of objects with modules information |
| ActiveModules | JsonArray | Array of objects with module instances information |
| Name  | string | The name of the module |
| Acronym | string | The acronym of the module |
| ID  | string | The ID of the module instance |

<br> Example:

``` JavaScript
{
    ResponseStatus: 1,
    MessageType: 2,
    AvailableModules: [
        {
            Name: "Cardiac",
            Acronym: "CRD"
        },
        {
            Name: "Brain",
            Acronym: "BRN"
        },
        ...     // As many as there is
    ],
    ActiveModules: [
        {
            Name: "Cardiac",
            ID: "Cardiac - 1"
        },
        {
            Name: "Cardiac",
            ID: "Cardiac - 2"
        },
        ...     // As many as there is
    ],
    Message: ""
}
```

## Requests

There are only two type of application requests, described as follows:

| Request Type | Value | Description |
| --- | --- | --- |
| Unknown     | 0 | Taken as an erroneous request |
| StartModule | 1 | Tells FI3D to activate the module |
| StopModule  | 2 | Tells FI3D to shut down the module |

<br>The requests have the additional keys:

| Key | Value Type | Description |
| --- | --- | --- |
| ApplicationParams | JsonObject | Contains parameters of the application request |
| ActionType | int | The type of application request, see previous table |
| ModuleName  | string | The name of the module to activate an instance of |
| ModuleID  | string | The ID of the module instance to shut down |

<br>Example of `StartModule` request:
``` JavaScript
{  
    MessageType: 2,
    ClientID: "FI-1",  
    ApplicationParams: {
        ActionType: 1,
        ModuleName: "Cardiac",
    },  
    Message: ""  
}  
```

<br>Example of `StopModule` request:
``` JavaScript
{  
    MessageType: 2,  
    ClientID: "FI-1",  
    ApplicationParams: {
        ActionType: 2,
        ModuleID: "Cardiac - 1"
    },  
    Message: ""
}  
```