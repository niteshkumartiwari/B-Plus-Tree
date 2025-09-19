public class Feature {
    private String description;
    private boolean isEnabled;
    public Feature(String description, boolean isEnabled) {
        this.description = description;
        this.isEnabled = isEnabled;
    }

    public String getDescription() {
        return description;
    }

    public boolean isEnabled() {
        return isEnabled;
    }

    public void setEnabled(boolean enabled) {
        isEnabled = enabled;
    }
}
