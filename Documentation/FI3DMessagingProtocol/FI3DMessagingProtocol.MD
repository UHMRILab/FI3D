# FI3D Messaging Protocol

The FI3D and connected devices, known as framework interfaces (FIs), communicate using this messaging protocol. The FIs need to authenticate with the system to ensure security. After authenticated, they can activate and stop modules, control active modules, and request data. A message (request and response) is identified by its type:

| Message Type | Value | Description |
| --- | --- | --- |
| Uknown            | 0 | Considered as an erronous message |
| Authentication    | 1 | For establishing connection with FI3D |
| Application       | 2 | Interactions with the overall framework |
| Module            | 3 | Control a specific module |
| Data              | 4 | Used to request data from the system |

<br>

## Responses:

All responses include a `ResponseStatus` key giving the state of the response. `ResponseStatus` can be one of the following:

| Response Status | Value |
| --- | --- |
| Success        | 1 |
| Error Response | 2 |
| Info Required  | 3 |

<br>

All FI3D responses have the following keys:

| Key | Value Type | Description |
| --- | --- | --- |
| ResponseStatus | int | Status of the response |
| MessageType | int | The type of response |
| ClientID  | string | The ID assigned by FI3D |
| Message  | string | A message to give to the FI user |

<br> Example:
``` JavaScript
{  
    ResponseStatus: 1,
    MessageType: 1,
    ClientID: "FI-0",      
    Message: "Hello FI-0!"
}  
```

## Requests:

All FI requests should have the following keys:

| Key | Value Type | Description |
| --- | --- | --- |
| MessageType | int | The type of request |
| ClientID  | string | The ID which was assigned by the FI3D |
| Message  | string | A message sent to the FI3D, this message is logged |

<br> Example:
``` JavaScript
{
    MessageType: 0,
    ClientID: "FI-0",
    Message: "message",
}
```

## Other Message Types

Those keys are always necessary, but additionally keys are needed, depending on the MessageType. Please seee the corresponding document for formatting of the four types of messages.

* [Authentication](AuthenticationMessages.MD)
* [Application](ApplicationMessages.MD)
* [Module](ModuleMessages.MD)
* [Data](DataMessages.MD)
