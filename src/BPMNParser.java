import java.io.File;
import java.util.Collection;

import org.camunda.bpm.model.bpmn.Bpmn;
import org.camunda.bpm.model.bpmn.BpmnModelInstance;
import org.camunda.bpm.model.bpmn.instance.Event;
import org.camunda.bpm.model.bpmn.instance.Gateway;
import org.camunda.bpm.model.bpmn.instance.Lane;
import org.camunda.bpm.model.bpmn.instance.Participant;
import org.camunda.bpm.model.bpmn.instance.Task;
import org.camunda.bpm.model.xml.instance.ModelElementInstance;
import org.camunda.bpm.model.xml.type.ModelElementType;

// javac -cp .;..\lib\* BPMNParser.java
// java -cp .;..\lib\* BPMNParser

public class BPMNParser {
	final public static String Path = "assets\\";
	final public static String file2 = Path + "busca-de-desaparecidos.bpmn";
	// final public static String file1 = Path + "alistamento-militar.bpmn";
	// final public static String file3 = Path + "cartao-sus.bpmn";
	// final public static String file4 = Path + "mario3mundo1.bpmn";
	// final public static String file5 = Path + "ProUNi.bpmn"; 

	public static void main(String[] args) throws Exception
	{
		startBPMNParser();
	}

	public static void startBPMNParser() throws Exception {
		System.out.println("BPM Game Engine from JAVA!!");

		java.util.Iterator<ModelElementInstance> iterator;
		
		String filename = file2;
		String name = filename.replace(Path, "");
		
		System.out.println("BPMN PARSER - CAMUNDA - " + filename);
		
		// pass the path to the file as a parameter 
	    File file = new File(filename);
	    BpmnModelInstance modelInstance = Bpmn.readModelFromFile(file);

	    System.out.println("Participantes");
	    ModelElementType particpantType = modelInstance.getModel().getType(Participant.class);
	    Collection<ModelElementInstance> participantInstances = modelInstance.getModelElementsByType(particpantType);
	    
	    iterator = participantInstances.iterator();
	    
	    while(iterator.hasNext()) {
	    	ModelElementInstance modelElementInstance = iterator.next();
	    	System.out.println("LANE: " + modelElementInstance.getAttributeValue("name"));
	    }
	    
	    System.out.println("\nSWIMLANES");
	    ModelElementType laneType = modelInstance.getModel().getType(Lane.class);
	    Collection<ModelElementInstance> laneInstances = modelInstance.getModelElementsByType(laneType);
	    
	    iterator = laneInstances.iterator();
	    
	    while(iterator.hasNext()) {
	    	ModelElementInstance modelElementInstance = iterator.next();
	    	System.out.println("LANE: " + modelElementInstance.getAttributeValue("name"));
	    }
	    
	    System.out.println("\nLista de eventos:");
	    ModelElementType eventType = modelInstance.getModel().getType(Event.class);
	    Collection<ModelElementInstance> eventInstances = modelInstance.getModelElementsByType(eventType);
	    
	    iterator = eventInstances.iterator();
	    
	    while(iterator.hasNext()) {
	    	ModelElementInstance modelElementInstance = iterator.next();
	    	System.out.println("EVENTO: " + modelElementInstance.getAttributeValue("name"));
	    }
	    
	    System.out.println("\nLista de tarefas:");
	    ModelElementType taskType = modelInstance.getModel().getType(Task.class);
	    Collection<ModelElementInstance> taskInstances = modelInstance.getModelElementsByType(taskType);
	    
	    iterator = taskInstances.iterator();
	    
	    while(iterator.hasNext()) {
	    	ModelElementInstance modelElementInstance = iterator.next();
	    	System.out.println("TAREFA: " + modelElementInstance.getAttributeValue("name"));
	    }
	    
	    System.out.println("\nLista de gateways:");
	    ModelElementType gatewayType = modelInstance.getModel().getType(Gateway.class);
	    Collection<ModelElementInstance> gatewayInstances = modelInstance.getModelElementsByType(gatewayType);
	    
	    iterator = gatewayInstances.iterator();
	    
	    while(iterator.hasNext()) {
	    	ModelElementInstance modelElementInstance = iterator.next();
	    	System.out.println("GATEWAY: " + modelElementInstance.getAttributeValue("id"));
	    }
	}
}