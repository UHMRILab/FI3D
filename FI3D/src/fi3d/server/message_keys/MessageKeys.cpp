#include <fi3d/server/message_keys/MessageKeys.h>
#include <QVector>

const QString fi3d::RESPONSE_STATUS = "ResponseStatus";
const QString fi3d::MESSAGE_TYPE = "MessageType";
const QString fi3d::CLIENT_ID = "ClientID";
const QString fi3d::MESSAGE = "Message";
const QString fi3d::PASSWORD = "Password";
const QString fi3d::APPLICATION_PARAMS = "ApplicationParams";
const QString fi3d::MODULE_PARAMS = "ModuleParams";
const QString fi3d::DATA_PARAMS = "DataParams";
const QString fi3d::ERROR_CODE = "ErrorCode";

const QString fi3d::ACTION_TYPE = "ActionType";
const QString fi3d::MODULE_NAME = "ModuleName";
const QString fi3d::NAME = "Name";
const QString fi3d::ACRONYM = "Acronym";
const QString fi3d::AVAILABLE_MODULES = "AvailableModules";
const QString fi3d::ACTIVE_MODULES = "ActiveModules";

const QString fi3d::MODULE_ID = "ModuleID";
const QString fi3d::REQUEST_ID = "RequestID";
const QString fi3d::TARGET_VISUAL = "TargetVisual";
const QString fi3d::IS_VISIBLE = "Visible";
const QString fi3d::TRANSLATE = "Translate";
const QString fi3d::ROTATE = "Rotate";
const QString fi3d::SLICE_INDEX = "SliceIndex";
const QString fi3d::SLICE_ORIENTATION = "SliceOrientation";
const QString fi3d::SERIES_INDEX = "SeriesIndex";
const QString fi3d::INTERACTION_ID = "InteractionID";
const QString fi3d::INTERACTION_VALUE = "InteractionValue";
const QString fi3d::RESPONSE_ID = "ResponseID";
const QString fi3d::MODULE_INFO = "ModuleInfo";
const QString fi3d::SCENE_INFO = "SceneInfo";
const QString fi3d::SCENE_ID = "SceneID";
const QString fi3d::VISUALS_INFO = "VisualsInfo";
const QString fi3d::MODULE_INTERACTIONS = "ModuleInteractions";
const QString fi3d::ID = "ID";
const QString fi3d::TYPE = "Type";
const QString fi3d::VALUE = "Value";
const QString fi3d::CONSTRAINT = "Constraint";
const QString fi3d::MIN_VALUE = "MinValue";
const QString fi3d::MAX_VALUE = "MaxValue";
const QString fi3d::VALUE_OPTIONS = "ValueOptions";
const QString fi3d::INFO = "Information";
const QString fi3d::VISUAL_INFO = "VisualInfo";
const QString fi3d::OPACITY = "Opacity";
const QString fi3d::TRANSFORMATION = "Transformation";
const QString fi3d::PARENT_ID = "ParentID";
const QString fi3d::DATA_ID = "DataID";
const QString fi3d::DATA_NAME = "DataName";
const QString fi3d::DATA_TYPE = "DataType";
const QString fi3d::COLOR = "Color";
const QString fi3d::ASSEMBLY_PART_COUNT = "AssemblyPartCount";
const QString fi3d::ASSEMBLY_ACTION = "AssemblyAction";
const QString fi3d::ASSEMBLY_PARTS_INFO = "AssemblyPartsInfo";
const QString fi3d::UPDATE_CONSTRAINT = "UpdateConstraint";

const QString fi3d::DATA = "Data";
const QString fi3d::CACHEABLE = "Cacheable";
const QString fi3d::SERIES_COUNT = "SeriesCount";
const QString fi3d::DIMENSIONS = "Dimensions";
const QString fi3d::ORIGIN = "Origin";
const QString fi3d::SPACING = "Spacing";
const QString fi3d::DATA_FORMAT = "DataFormat";
const QString fi3d::VALUES = "Values";
const QString fi3d::POINTS = "Points";
const QString fi3d::LINE_INDICES = "LineIndices";
const QString fi3d::TRIANGLE_INDICES = "TriangleIndices";
const QString fi3d::PAYLOAD_POINTS_LENGTH = "PayloadPointsLength";
const QString fi3d::PAYLOAD_TRIANGLES_LENGTH = "PayloadTrianglesLength";

// TODO: Payload texture length, payload texture coordinate length
const QString fi3d::PAYLOAD_TCOORDS_LENGTH = "PayloadTCoordsLength";
const QString fi3d::PAYLOAD_TEXTURES_COUNT = "PayloadTexturesCount";
const QVector<QString> fi3d::PAYLOAD_TEXTURES_LENGTH = 
						{ "PayloadTextureLength0"
						, "PayloadTextureLength1"
						, "PayloadTextureLength2"
						, "PayloadTextureLength3"
						, "PayloadTextureLength4" };

const QVector<QString> fi3d::PAYLOAD_TEXTURES_DIMENSION_WIDTH = 
						{ "PayloadTextureDimensionWidth0"
						, "PayloadTextureDimensionWidth1"
						, "PayloadTextureDimensionWidth2"
						, "PayloadTextureDimensionWidth3"
						, "PayloadTextureDimensionWidth4" };

const QVector<QString> fi3d::PAYLOAD_TEXTURES_DIMENSION_HEIGHT = 
						{ "PayloadTextureDimensionHeight0"
						, "PayloadTextureDimensionHeight1"
						, "PayloadTextureDimensionHeight2"
						, "PayloadTextureDimensionHeight3"
						, "PayloadTextureDimensionHeight4" };