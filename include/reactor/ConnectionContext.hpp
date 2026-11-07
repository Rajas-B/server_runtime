// abstract class to pass to Protocol Handler to allow callbacks
class ConnectionContext {
    // these 2 are specific HTTP 1.1
    virtual void pause_reading() = 0;
    virtual void resume_reading() = 0;
};