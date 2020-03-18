<?php

class Column {
    public $name;
    public $format;
    public $ref;

    public function __construct( $name, $format, $ref )
    {
        $this->name = $name;
        $this->format = $format;
        $this->ref = $ref;
    }
}

class TableGenerator {
    private $cols;
    private $data;

    public function addColumn($name, $format, $ref)
    {
        $col = new Column( $name, $format, $ref );
        $this->cols[] = $col;
    }

    public function setData( $cardsincollection )
    {
        $this->data = $cardsincollection;
    }

    public function generateHTML()
    {
        echo '<table class="sortable">';
        // populate headers
        $csize = count($this->cols);
        $collectioncount = count($this->data);
        echo '<tr>';
        for($i = 0; $i < $csize; $i++ ) {
            echo '<th>'.$this->cols[$i]->name.'</th>';
        }
        echo '</tr>';
        // populate rows
        for ( $c = 0; $c < $collectioncount; $c++ ) {
            echo '<tr>';
            for($i = 0; $i < $csize; $i++ ) {
                echo '<td>';
                foreach ($this->cols[ $i ]->ref as $key=>$value ) {
                    $ret[] = $this->data[ $c ][ $value ];
                }
                vprintf($this->cols[ $i ]->format, $ret);
                unset($ret);
                echo '</td>';
            }
            echo '</tr>';
        }
        echo '</table>';
    }
}

?>
