# FI3D Messaging Protocol - Authentication Messages

Authentication messages are the first type of messages exchanged by the FI3D and its FIs. All FIs are required to authenticate to ensure security of the system. When an FI establishes the TCP connection with the FI3D, the FI3D sends a response which asks to provide authentication information (password) and gives the assigned FI a `Client ID` used to identify the FI. Once the FI provides the correct password, the FI is authenticated and is allowed to make the other three type of requests.

## Responses

Responses have no additional keys. When the FI3D asks the FI to provide the password, the response looks as follows:

``` JavaScript
{  
    ResponseStatus: 3,  
    MessageType: 1,
    ClientID: "FI-0",
    Message: ""
}  
```

When the user provides the correct password, the FI will receive the following response:

``` JavaScript
{  
    ResponseStatus: 1,
    MessageType: 1,
    ClientID: "FI-0",
    Message: "Authentication Successful!"
}  
```

After receiving this message, there is no more use for `Authentication` messages between the two devices.

## Requests

When the user gets the response that asks for a password, the FI should send a request providing the password by adding the `Password` key to the request, as shown:

``` JavaScript
{
    MessageType: 1,
    ClientID: "FI-0",
    Password: "admin",
    Message: "",
}
```

If the `Password` is correct, the FI should receive a response that looks like the 2nd response above in the Responses section, completing the authentication exchange of the connection.