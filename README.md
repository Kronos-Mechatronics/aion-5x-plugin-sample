# Aion-5X-plugin-sample
Reference implementation of a Cam extension plugin for Aion-5X

## Build the plugin
1. Install Rhino, Aion-5X, Rhino SDK and required developer tools: [Installing Tools (Windows)](https://developer.rhino3d.com/guides/cpp/installing-tools-windows/)
2. Download and extract Aion-5X SDK
3. Clone this repository into the same directory that holds the Aion-5X SDK

Example directory tree:

```
├── C:\software\
│   ├── Aion-5X-SDK
│   │   ├── include\
│   │   ├── lib\
│   │   ├── PropertySheets\
│   ├── aion-5x-plugin-sample\
│   │   ├── CamDemo\
│   │   ├── CamDemo.sln
```

4. Open `CamDemo.sln` with Visual Studio and build the project
5. Install the resulting `CamDemo.rhp` file in Rhino